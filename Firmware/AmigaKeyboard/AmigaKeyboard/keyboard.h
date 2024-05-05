/*
 * keyboard.h
 *
 * Created: 14/01/2022 12:17:07
 *  Author: ebakke
 */ 

#include <avr/io.h>

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#define NUM_ROWS	( 6 )
#define NUM_COLS	( 16 )

// Keyboard serial
#define KBDATA_OUT		( PORTB )
#define KBDATA_IN		( PINB )
#define KBDATA_DDR		( DDRB )
#define KBDATA_PIN		( 0 )
#define KBDATA_MASK		( 1 << KBDATA_PIN )

#define KBCLK_OUT		( PORTB )
#define KBCLK_DDR		( DDRB )
#define KBCLK_PIN		( 1 )
#define KBCLK_MASK		( 1 << KBCLK_PIN )

#define KBRESET_OUT		( PORTB )
#define KBRESET_DDR		( DDRB )
#define KBRESET_PIN		( 2 )
#define KBRESET_MASK	( 1 << KBRESET_PIN )

// Column signals
#define COL00_IN		( PINB )
#define COL00_DDR		( DDRB )
#define COL00_PIN		( 3 )
#define COL00_MASK		( 1 << COL00_PIN )

#define COL01_IN		( PINB )
#define COL01_DDR		( DDRB )
#define COL01_PIN		( 7 )
#define COL01_MASK		( 1 << COL01_PIN )

#define COL02_IN		( PIND )
#define COL02_DDR		( DDRD )
#define COL02_PIN		( 0 )
#define COL02_MASK		( 1 << COL02_PIN )

#define COL03_IN		( PIND )
#define COL03_DDR		( DDRD )
#define COL03_PIN		( 1 )
#define COL03_MASK		( 1 << COL03_PIN )

#define COL04_IN		( PIND )
#define COL04_DDR		( DDRD )
#define COL04_PIN		( 2 )
#define COL04_MASK		( 1 << COL04_PIN )

#define COL05_IN		( PIND )
#define COL05_DDR		( DDRD )
#define COL05_PIN		( 3 )
#define COL05_MASK		( 1 << COL05_PIN )

#define COL06_IN		( PIND )
#define COL06_DDR		( DDRD )
#define COL06_PIN		( 5 )
#define COL06_MASK		( 1 << COL06_PIN )

#define COL07_IN		( PIND )
#define COL07_DDR		( DDRD )
#define COL07_PIN		( 4 )
#define COL07_MASK		( 1 << COL07_PIN )

#define COL08_IN		( PIND )
#define COL08_DDR		( DDRD )
#define COL08_PIN		( 6 )
#define COL08_MASK		( 1 << COL08_PIN )

#define COL09_IN		( PIND )
#define COL09_DDR		( DDRD )
#define COL09_PIN		( 7 )
#define COL09_MASK		( 1 << COL09_PIN )

#define COL10_IN		( PINB )
#define COL10_DDR		( DDRB )
#define COL10_PIN		( 4 )
#define COL10_MASK		( 1 << COL10_PIN )

#define COL11_IN		( PINF )
#define COL11_DDR		( DDRF )
#define COL11_PIN		( 6 )
#define COL11_MASK		( 1 << COL11_PIN )

#define COL12_IN		( PINF )
#define COL12_DDR		( DDRF )
#define COL12_PIN		( 5 )
#define COL12_MASK		( 1 << COL12_PIN )

#define COL13_IN		( PINF )
#define COL13_DDR		( DDRF )
#define COL13_PIN		( 4 )
#define COL13_MASK		( 1 << COL13_PIN )

#define COL14_IN		( PINF )
#define COL14_DDR		( DDRF )
#define COL14_PIN		( 1 )
#define COL14_MASK		( 1 << COL14_PIN )

#define COL15_IN		( PINF )
#define COL15_DDR		( DDRF )
#define COL15_PIN		( 0 )
#define COL15_MASK		( 1 << COL15_PIN )

// Row signals
#define ROW00_OUT		( PORTC )
#define ROW00_DDR		( DDRC )
#define ROW00_PIN		( 7 )
#define ROW00_MASK		( 1 << ROW00_PIN )

#define ROW01_OUT		( PORTE )
#define ROW01_DDR		( DDRE )
#define ROW01_PIN		( 2 )
#define ROW01_MASK		( 1 << ROW01_PIN )

#define ROW02_OUT		( PORTC )
#define ROW02_DDR		( DDRC )
#define ROW02_PIN		( 6 )
#define ROW02_MASK		( 1 << ROW02_PIN )

#define ROW03_OUT		( PORTF )
#define ROW03_DDR		( DDRF )
#define ROW03_PIN		( 7 )
#define ROW03_MASK		( 1 << ROW03_PIN )

#define ROW04_OUT		( PORTB )
#define ROW04_DDR		( DDRB )
#define ROW04_PIN		( 6 )
#define ROW04_MASK		( 1 << ROW04_PIN )

#define ROW05_OUT		( PORTB )
#define ROW05_DDR		( DDRB )
#define ROW05_PIN		( 5 )
#define ROW05_MASK		( 1 << ROW05_PIN )

// Caps Lock
#define CAPS_OUT		( PORTE )
#define CAPS_DDR		( DDRE )
#define CAPS_PIN		( 6 )
#define CAPS_MASK		( 1 << CAPS_PIN )

// Keyboard scan codes
#define KEY_BTICK		( 0x00 )
#define KEY_1			( 0x01 )
#define KEY_2			( 0x02 )
#define KEY_3			( 0x03 )
#define KEY_4			( 0x04 )
#define KEY_5			( 0x05 )
#define KEY_6			( 0x06 )
#define KEY_7			( 0x07 )
#define KEY_8			( 0x08 )
#define KEY_9			( 0x09 )
#define KEY_0			( 0x0a )
#define KEY_MINUS		( 0x0b )
#define KEY_EQUALS		( 0x0c )
#define KEY_BSLASH		( 0x0d )
#define KEY_NUM0		( 0x0f )
#define KEY_Q			( 0x10 )
#define KEY_W			( 0x11 )
#define KEY_E			( 0x12 )
#define KEY_R			( 0x13 )
#define KEY_T			( 0x14 )
#define KEY_Y			( 0x15 )
#define KEY_U			( 0x16 )
#define KEY_I			( 0x17 )
#define KEY_O			( 0x18 )
#define KEY_P			( 0x19 )
#define KEY_LBRK		( 0x1a )
#define KEY_RBRK		( 0x1b )
#define KEY_NUM1		( 0x1d )
#define KEY_NUM2		( 0x1e )
#define KEY_NUM3		( 0x1f )
#define KEY_A			( 0x20 )
#define KEY_S			( 0x21 )
#define KEY_D			( 0x22 )
#define KEY_F			( 0x23 )
#define KEY_G			( 0x24 )
#define KEY_H			( 0x25 )
#define KEY_J			( 0x26 )
#define KEY_K			( 0x27 )
#define KEY_L			( 0x28 )
#define KEY_SEMI		( 0x29 )
#define KEY_APOS		( 0x2a )
#define KEY_INTER1		( 0x2b )
#define KEY_NUM4		( 0x2d )
#define KEY_NUM5		( 0x2e )
#define KEY_NUM6		( 0x2f )
#define KEY_INTER2		( 0x30 )
#define KEY_Z			( 0x31 )
#define KEY_X			( 0x32 )
#define KEY_C			( 0x33 )
#define KEY_V			( 0x34 )
#define KEY_B			( 0x35 )
#define KEY_N			( 0x36 )
#define KEY_M			( 0x37 )
#define KEY_COMMA		( 0x38 )
#define KEY_DOT			( 0x39 )
#define KEY_SLASH		( 0x3a )
#define KEY_NUMDOT		( 0x3c )
#define KEY_NUM7		( 0x3d )
#define KEY_NUM8		( 0x3e )
#define KEY_NUM9		( 0x3f )
#define KEY_SPACE		( 0x40 )
#define KEY_BSPACE		( 0x41 )
#define KEY_TAB			( 0x42 )
#define KEY_NUMENTER	( 0x43 )
#define KEY_RETURN		( 0x44 )
#define KEY_ESC			( 0x45 )
#define KEY_DEL			( 0x46 )
#define KEY_NUMMINUS	( 0x4a )
#define KEY_UP			( 0x4c )
#define KEY_DOWN		( 0x4d )
#define KEY_RIGHT		( 0x4e )
#define KEY_LEFT		( 0x4f )
#define KEY_F1			( 0x50 )
#define KEY_F2			( 0x51 )
#define KEY_F3			( 0x52 )
#define KEY_F4			( 0x53 )
#define KEY_F5			( 0x54 )
#define KEY_F6			( 0x55 )
#define KEY_F7			( 0x56 )
#define KEY_F8			( 0x57 )
#define KEY_F9			( 0x58 )
#define KEY_F10			( 0x59 )
#define KEY_NUMLPAR		( 0x5a )
#define KEY_NUMRPAR		( 0x5b )
#define KEY_NUMSLASH	( 0x5c )
#define KEY_NUMSTAR		( 0x5d )
#define KEY_NUMPLUS		( 0x5e )
#define KEY_HELP		( 0x5f )
#define KEY_LSHIFT		( 0x60 )
#define KEY_RSHIFT		( 0x61 )
#define KEY_CAPS		( 0x62 )
#define KEY_CTRL		( 0x63 )
#define KEY_LALT		( 0x64 )
#define KEY_RALT		( 0x65 )
#define KEY_LAMIGA		( 0x66 )
#define KEY_RAMIGA		( 0x67 )

// Special purpose scan codes
#define KEY_LASTKEYBAD	( 0xf9 )
#define KEY_INITPUKEYS	( 0xfd )
#define KEY_TERMPUKEYS	( 0xfe )

// ROW/COL mappings for keys that need special attention
#define LAMIGA_ROW		( 5 )
#define LAMIGA_COL		( 2 )
#define RAMIGA_ROW		( 5 )
#define RAMIGA_COL		( 4 )
#define CTRL_ROW		( 3 )
#define CTRL_COL		( 0 )
#define CAPS_ROW		( 3 )
#define CAPS_COL		( 1 )

// Convenience macros for converting row/col to packed status map indices
#define KEYSTATUSMAP_IDX( row, col ) ( (uint8_t)( ( ( ( row ) * NUM_COLS ) + ( col ) ) >> 3 ) )
#define KEYSTATUSMAP_MASK( col ) ( (uint8_t)( 1 << ( ( col ) & 0x7 ) ) )

// Convenience functions for working with GPIO
static inline void set_caps_high( void )
{
	CAPS_OUT |= CAPS_MASK;
}

static inline void set_caps_low( void )
{
	CAPS_OUT &= ~CAPS_MASK;
}

// Function prototypes
inline void set_kbdata_high( void )
{
	KBDATA_OUT |= KBDATA_MASK;
}

inline void set_kbdata_low( void )
{
	KBDATA_OUT &= ~KBDATA_MASK;
}

inline bool get_kbdata( void )
{
	return ( ( KBDATA_IN & KBDATA_MASK ) != 0 );
}

inline void config_kbdata_input( void )
{
	KBDATA_DDR &= ~KBDATA_MASK;
}

inline void config_kbdata_output( void )
{
	KBDATA_DDR |= KBDATA_MASK;
}

inline void config_kbdata_pullup_enable( void )
{
	KBDATA_OUT |= KBDATA_MASK;
}

inline void config_kbdata_pullup_disable( void )
{
	KBDATA_OUT &= ~KBDATA_MASK;
}

inline void set_kbclk_high( void )
{
	KBCLK_OUT |= KBCLK_MASK;
}

inline void set_kbclk_low( void )
{
	KBCLK_OUT &= ~KBCLK_MASK;
}

inline void set_kbreset_high( void )
{
	KBRESET_OUT |= KBRESET_MASK;
}

inline void set_kbreset_low( void )
{
	KBRESET_OUT &= ~KBRESET_MASK;
}

inline bool get_col00( void )
{
	return ( ( COL00_IN & COL00_MASK ) != 0 );
}

inline bool get_col01( void )
{
	return ( ( COL01_IN & COL01_MASK ) != 0 );
}

inline bool get_col02( void )
{
	return ( ( COL02_IN & COL02_MASK ) != 0 );
}

inline bool get_col03( void )
{
	return ( ( COL03_IN & COL03_MASK ) != 0 );
}

inline bool get_col04( void )
{
	return ( ( COL04_IN & COL04_MASK ) != 0 );
}

inline bool get_col05( void )
{
	return ( ( COL05_IN & COL05_MASK ) != 0 );
}

inline bool get_col06( void )
{
	return ( ( COL06_IN & COL06_MASK ) != 0 );
}

inline bool get_col07( void )
{
	return ( ( COL07_IN & COL07_MASK ) != 0 );
}

inline bool get_col08( void )
{
	return ( ( COL08_IN & COL08_MASK ) != 0 );
}

inline bool get_col09( void )
{
	return ( ( COL09_IN & COL09_MASK ) != 0 );
}

inline bool get_col10( void )
{
	return ( ( COL10_IN & COL10_MASK ) != 0 );
}

inline bool get_col11( void )
{
	return ( ( COL11_IN & COL11_MASK ) != 0 );
}

inline bool get_col12( void )
{
	return ( ( COL12_IN & COL12_MASK ) != 0 );
}

inline bool get_col13( void )
{
	return ( ( COL13_IN & COL13_MASK ) != 0 );
}

inline bool get_col14( void )
{
	return ( ( COL14_IN & COL14_MASK ) != 0 );
}

inline bool get_col15( void )
{
	return ( ( COL15_IN & COL15_MASK ) != 0 );
}

inline void set_row00_high( void )
{
	ROW00_OUT |= ROW00_MASK;
}

inline void set_row00_low( void )
{
	ROW00_OUT &= ~ROW00_MASK;
}

inline void set_row01_high( void )
{
	ROW01_OUT |= ROW01_MASK;
}

inline void set_row01_low( void )
{
	ROW01_OUT &= ~ROW01_MASK;
}

inline void set_row02_high( void )
{
	ROW02_OUT |= ROW02_MASK;
}

inline void set_row02_low( void )
{
	ROW02_OUT &= ~ROW02_MASK;
}

inline void set_row03_high( void )
{
	ROW03_OUT |= ROW03_MASK;
}

inline void set_row03_low( void )
{
	ROW03_OUT &= ~ROW03_MASK;
}

inline void set_row04_high( void )
{
	ROW04_OUT |= ROW04_MASK;
}

inline void set_row04_low( void )
{
	ROW04_OUT &= ~ROW04_MASK;
}

inline void set_row05_high( void )
{
	ROW05_OUT |= ROW05_MASK;
}

inline void set_row05_low( void )
{
	ROW05_OUT &= ~ROW05_MASK;
}

void keyboard_init( void );
void scanMatrix( void );

#endif /* KEYBOARD_H_ */