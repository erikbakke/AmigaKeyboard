/*
 * keyboard.c
 *
 * Created: 14/01/2022 11:56:59
 *  Author: ebakke
 */ 
#include <avr/io.h>
#include <stdbool.h>
#include <progmem.h>
#include <delay.h>
#include <interrupt.h>

#include "keyboard.h"
#include "debug.h"

// Used for debugging purposes
static void flashLED( int durationMS, uint8_t count )
{
	for( int i = 0; i < count; i++ )
	{
		set_caps_high();
		delay_ms( durationMS );
		set_caps_low();
		delay_ms( durationMS );
	}
}

// Note that the matrix layout is different compared to the standard Commodore keyboards.
// However, the scan codes for each key matches those of the original keyboards.
static const uint8_t scancodes[ NUM_ROWS * NUM_COLS ] =
{
	// COL00    COL01     COL02       COL03      COL04       COL05     COL06     COL07     COL08        COL09     COL10      COL11       COL12       COL13        COL14         COL15
	// ROW 00:
	KEY_ESC,    KEY_F1,   KEY_F2,     KEY_F3,    KEY_F4,     KEY_F5,   KEY_F6,   KEY_F7,   KEY_F8,      KEY_F9,   KEY_F10,   KEY_DEL,    KEY_HELP,   KEY_NUMLPAR, KEY_NUMRPAR,  KEY_NUMSLASH,
	// ROW 01:
	KEY_BTICK,  KEY_1,    KEY_2,      KEY_3,     KEY_4,      KEY_5,    KEY_6,    KEY_7,    KEY_8,       KEY_9,    KEY_0,     KEY_MINUS,  KEY_EQUALS, KEY_BSLASH,  KEY_BSPACE,   KEY_NUMSTAR,
	// ROW 02:
	KEY_TAB,    KEY_Q,    KEY_W,      KEY_E,     KEY_R,      KEY_T,    KEY_Y,    KEY_U,    KEY_I,       KEY_O,    KEY_P,     KEY_LBRK,   KEY_RBRK,   KEY_NUM7,    KEY_NUM8,     KEY_NUM9,
	// ROW 03:
	KEY_CTRL,   KEY_CAPS, KEY_A,      KEY_S,     KEY_D,      KEY_F,    KEY_G,    KEY_H,    KEY_J,       KEY_K,    KEY_L,     KEY_SEMI,   KEY_APOS,   KEY_INTER1,  KEY_RETURN,   KEY_NUMMINUS,
	// ROW 04:
	KEY_INTER2, KEY_Z,    KEY_X,      KEY_C,     KEY_V,      KEY_B,    KEY_N,    KEY_M,    KEY_COMMA,   KEY_DOT,  KEY_SLASH, KEY_RSHIFT, KEY_LEFT,   KEY_DOWN,    KEY_RIGHT,    KEY_UP,
	// ROW 05:
	KEY_LSHIFT, KEY_LALT, KEY_LAMIGA, KEY_SPACE, KEY_RAMIGA, KEY_RALT, KEY_NUM5, KEY_NUM6, KEY_NUMPLUS, KEY_NUM1, KEY_NUM2,  KEY_NUM3,   KEY_NUM0,   KEY_NUMDOT,  KEY_NUMENTER, KEY_NUM4
};

// Current status of each key, switches are active low so 0 means pressed, 1 released
static uint8_t keyStatus[ ( NUM_ROWS * NUM_COLS ) >> 3 ];
// Separate active low flag for caps lock, as this is a toggle and the state does not correspond directly to the press/release state
static bool capsLockStatus = true;

// Set the key status, active low
static inline void setKeyStatus( uint8_t row, uint8_t col, bool _pressed )
{
	// _pressed is active low, so a true value indicates the key has been RELEASED, and we'll need to SET the keyStatus to true
	if( _pressed )
	{
		keyStatus[ KEYSTATUSMAP_IDX( row, col ) ] |= KEYSTATUSMAP_MASK( col );
	}
	else
	{
		keyStatus[ KEYSTATUSMAP_IDX( row, col ) ] &= ~KEYSTATUSMAP_MASK( col );
	}
}

// Get the key status, active low
static inline bool getKeyStatus( uint8_t row, uint8_t col )
{
	return ( ( keyStatus[ KEYSTATUSMAP_IDX( row, col ) ] & KEYSTATUSMAP_MASK( col ) ) != 0 );
}

static inline uint8_t getScanCode( uint8_t row, uint8_t col )
{
	return scancodes[ ( row * NUM_COLS ) + col ];
}

static inline bool waitForHandshake( void )
{
	bool bit;
	unsigned char ackWait = 143;	// 143 msecs * 1000 samples
	unsigned int sample;
	bool gotHandshake = false;

	// After sending the last bit, wait for the Amiga bring KBDATA low to acknowledge the event.  To do this, we'll need
	// to switch KBDATA to an input and make sure the pullup is disabled
	config_kbdata_input();
	config_kbdata_pullup_enable();
	
	// The Amiga has to acknowledge within 143 msecs, otherwise the interface is considered to have become desynchronised
	// This is done by pulling KBDATA low for at least 1 usec.  To ensure we have enough temporal resolution for this,
	// we divide this into 143 msecs of 1000 samples each, so each test is 1 usec.
	do
	{
		sample = 1000;
		do
		{
			bit = get_kbdata();
			if( !bit )	// KBDATA is active low, so break if we get a low
			{
				gotHandshake = true;
				break;
			}
			// The HRM specifies we should wait for up to 143 msecs for the handshake.  There are enough instructions in this
			// loop that we are quite close to that at the end of the loop
		}
		while( --sample );

		if( gotHandshake )
		{
			break;
		}
	}
	while( --ackWait );
	
	// Change KBDATA back to an output and set high
	config_kbdata_output();
	set_kbdata_high();

	return ( ackWait >= 0 );	// If ackWait is greater than zero, we got a handshake before the 143ms timed out
}

static bool serialOut( uint8_t data )
{
	bool bit;
	
	// We start out with _KBDATA and _KBCLK both high
	set_kbdata_high();
	set_kbclk_high();

	// The transmission bit order is 6, 5, 4, 3, 2, 1, 0, 7, so we'll need to rotate the data first
	uint8_t rotatedData = ( data << 1 )	| ( ( data & 0x80 ) >> 7 );
	
	// Then we shift out each bit of data, most significant bit first
	int i = 7;
	do
	{
		bit = ( ( rotatedData & ( 1 << i ) ) != 0 );
		
		// We set the data (active low, so inversed) on KBDATA, and 20 usecs later we pull KBCLK low for 20 usecs then back to high.
		// Finally, we wait for another 20 usecs before we send the next bit
		if( bit )
		{
			set_kbdata_low();
		}
		else
		{
			set_kbdata_high();
		}
		delay_us( 20 );		// Wait 20 usecs before bringing KBCLK low
		set_kbclk_low();	// Bring KBCLK low
		delay_us( 20 );		// Keep it low for 20 usecs
		set_kbclk_high();	// And set it high again
		delay_us( 20 );		// Are you keeping up with the Commodore?
		i--;
	}
	while( i >= 0 );
	
	// Set the KBDATA high again after the last bit
	set_kbdata_high();
	// We probably don't need to wait here, as waits will happen in waitForHandshake() anyway.  Removing the delay also removes the risk that the
	// handshake happens before we have a chance to wait for it.
	//delay_us( 20 );

	return waitForHandshake();
}

static bool resync( void )
{
	debug( "resync()\n " );
	
	set_kbdata_low();
	delay_us( 20 );		// Wait 20 usecs before bringing KBCLK low
	
	set_kbclk_low();
	delay_us( 20 );	// The HRM doesn't say explicitly to wait 20 usecs, but we'll do as when sending events.
	set_kbclk_high();
	delay_us( 20 );
	set_kbdata_high();
	
	return waitForHandshake();	// Return true if the handshake was received, false otherwise	
}

static void sendEvent( uint8_t scanCode, bool pressed )
{
	debugf( "sendEvent( scanCode = %02x, pressed = %d\n", scanCode, pressed );
	/*
	 * The high bit of the event is a press/release flag, followed by 7 bits of the scan code
	 * To send the event, we send the 8 bits and then wait for the computer to acknowledge by bringing KBDATA low at its end.
	 * We will wait for up to 143 ms for this acknowledgement.  If it doesn't happen, we will initiate the resync protocol and attempt to resend the event.
	 * This will continue until an acknowledgement is received.
	 *
	 * Resets are handled differently, see below.
	 */
	uint8_t data = ( pressed ? 0x80 : 0x00 ) | scanCode;
	
	while( !serialOut( data ) )
	{
		// Serial send failed, attempt to resync.  Note that this could potentially continue forever which, strangely enough, is according to the protocol.
		// However, if we're debugging, we'll give up after 100 iterations
		#ifdef DEBUG
		int cnt = 100;
		while( !resync() && ( cnt-- >= 0 ) );
		#else
		while( !resync() );
		#endif
		// Once we have reestablished sync, tell the Amiga the previous character received was junk and that we will resend, then let the while-loop resend the code
		serialOut( KEY_LASTKEYBAD );
	}
}

void keyboard_init( void )
{
	debug( "keyboard_init()\n" );
	// Column signals are configured as inputs with pull ups.  Rows are outputs
	DDRB = KBDATA_MASK | KBCLK_MASK | KBRESET_MASK | ROW04_MASK | ROW05_MASK;
	DDRC = ROW00_MASK | ROW02_MASK;
	DDRD = 0;	// All inputs
	DDRE = ROW01_MASK | CAPS_MASK;
	DDRF = ROW03_MASK;
	
	// Enable pull-ups for column signal inputs, set row signals high, Caps Lock LED low, and keyboard serial outputs high
	PORTB = COL00_MASK | COL01_MASK | COL10_MASK| ROW04_MASK | ROW05_MASK | KBDATA_MASK | KBCLK_MASK | KBRESET_MASK;
	PORTC = ROW00_MASK | ROW02_MASK;
	PORTD = COL02_MASK | COL03_MASK | COL04_MASK | COL05_MASK | COL06_MASK | COL07_MASK | COL08_MASK | COL09_MASK;
	PORTE = ROW01_MASK;
	PORTF = COL11_MASK | COL12_MASK | COL13_MASK | COL14_MASK | COL15_MASK | ROW03_MASK;
	
	// Initialise all key status values to released
	for( int i = 0; i < sizeof( keyStatus ); i++ )
	{
		keyStatus[ i ] = 0xff;
	}
	
	// Once the hardware has been set up, initiate the power-up protocol, which is simply to keep sending sync to
	// the Amiga until it responds with a handshake.
	while( !resync() );

	// After establishing sync, send the power-up key stream, which is 0xfd, all keys currently held down, and finally 0xfe
	serialOut( KEY_INITPUKEYS );
	scanMatrix();
	serialOut( KEY_TERMPUKEYS );
	
	debug( "keyboard_init() done\n" );
}

static inline void sendKeyPress( uint8_t row, uint8_t col )
{
	debugf( "sendKeyPress( row = %d, col = %d\n", row, col );
	sendEvent( getScanCode( row, col ), false );
}

static inline void sendKeyRelease( uint8_t row, uint8_t col )
{
	debugf( "sendKeyRelease( row = %d, col = %d\n", row, col );
	sendEvent( getScanCode( row, col ), true );
}

static inline bool reset_requested( void )
{
	// This is flagged by the 3 keys Ctrl, LAmiga and RAmiga all having a low value
	return(    !getKeyStatus( 3, 0 )
	        && !getKeyStatus( 5, 2 )
			&& !getKeyStatus( 5, 4 ) );
}

// To scan the keyboard matrix, we bring each row low in turn, leaving the other rows high.
// Then we check the input value of each column in turn.  A pressed key at that matrix position will show up
// as a low signal
void scanMatrix( void )
{
	for( int row = 0; row < NUM_ROWS; row++ )
	{
		switch( row )
		{
			case 0:
				set_row00_low();
				break;
				
			case 1:
				set_row01_low();
				break;
				
			case 2:
				set_row02_low();
				break;
				
			case 3:
				set_row03_low();
				break;
				
			case 4:
				set_row04_low();
				break;
				
			case 5:
				set_row05_low();
				break;
		}
		for( int col = 0; col < NUM_COLS; col++ )
		{
			bool switchStatus = true;
			
			switch( col )
			{
				case 0:
					switchStatus = get_col00();
					break;
					
				case 1:
					switchStatus = get_col01();
					break;
					
				case 2:
					switchStatus = get_col02();
					break;
					
				case 3:
					switchStatus = get_col03();
					break;
					
				case 4:
					switchStatus = get_col04();
					break;
					
				case 5:
					switchStatus = get_col05();
					break;
					
				case 6:
					switchStatus = get_col06();
					break;
					
				case 7:
					switchStatus = get_col07();
					break;
					
				case 8:
					switchStatus = get_col08();
					break;
					
				case 9:
					switchStatus = get_col09();
					break;
					
				case 10:
					switchStatus = get_col10();
					break;
					
				case 11:
					switchStatus = get_col11();
					break;
					
				case 12:
					switchStatus = get_col12();
					break;
					
				case 13:
					switchStatus = get_col13();
					break;
					
				case 14:
					switchStatus = get_col14();
					break;
					
				case 15:
					switchStatus = get_col15();
					break;
			}
			
			bool previousStatus = getKeyStatus( row, col );
			
			// To speed things up, we first check if the status has changed, and then check the new status.  This way, we can get away
			// with fewer comparisons for the keys that have not changed.
			if( previousStatus != switchStatus )
			{
				// Caps Lock doesn't send key release events like the other keys.  It changes state every time it is pressed
				if( ( row == CAPS_ROW ) && ( col == CAPS_COL ) )
				{
					// Caps Lock toggles when pressed, and nothing is sent on release
					if( !switchStatus )
					{
						if( capsLockStatus )
						{
							// Caps Lock was not active, so send the press event
							sendKeyPress( row, col );
						}
						else
						{
							// Caps Lock was active, so send the release event
							sendKeyRelease( row, col );
						}
						capsLockStatus = !capsLockStatus;
						
						// Caps lock status is active low, so activate the C/L LED when low
						if( capsLockStatus )
						{
							set_caps_low();
						}
						else
						{
							set_caps_high();
						}
					}
				}
				else
				{
					if( switchStatus )
					{
						// The key has been released
						sendKeyRelease( row, col );
					}
					else
					{
						// The key has been pressed
						sendKeyPress( row, col );
					}
				}
				
				setKeyStatus( row, col, switchStatus );
			}
		}

		// Bring the row lines high again
		switch( row )
		{
			case 0:
			set_row00_high();
			break;
			
			case 1:
			set_row01_high();
			break;
			
			case 2:
			set_row02_high();
			break;
			
			case 3:
			set_row03_high();
			break;
			
			case 4:
			set_row04_high();
			break;
			
			case 5:
			set_row05_high();
			break;
		}
	}
			
	// Check if the user has requested a reset by pressing Ctrl+LAmiga+RAmiga, a.k.a. the 3-fingered salute.
	if( reset_requested() )
	{
		debug( "reset_requested()\n" );
		
		// For an A500, we will assert KBRESET by bringing that line low.
		// For models with an external keyboard, this works a bit differently in that we need to bring the KBCLK line low for at least 500 msecs.
		// (We'll keep it low for 501 msecs, just in case)
		// After this, the MCU should initialise as after a cold start, so we'll just rerun the initialisation code.
		set_kbreset_low();

		set_kbclk_low();
		delay_ms( 501 );
		set_kbclk_high();
		set_kbreset_high();
		keyboard_init();
	}
}
