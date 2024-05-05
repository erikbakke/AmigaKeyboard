/*
 * usb.c
 *
 * Created: 16/02/2022 20:41:48
 *  Author: ebakke
 */ 

#include <avr/wdt.h>
#include <avr/power.h>
#include <ASF/common/services/delay/delay.h>
#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Platform/Platform.h>
#include "Descriptors.h"
#include "usb.h"
#include "debug.h"

static USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
{
	.Config =
	{
		.ControlInterfaceNumber   = INTERFACE_ID_CDC_CCI,
		.DataINEndpoint           =
		{
			.Address          = CDC_TX_EPADDR,
			.Size             = CDC_TXRX_EPSIZE,
			.Banks            = 1,
		},
		.DataOUTEndpoint =
		{
			.Address          = CDC_RX_EPADDR,
			.Size             = CDC_TXRX_EPSIZE,
			.Banks            = 1,
		},
		.NotificationEndpoint =
		{
			.Address          = CDC_NOTIFICATION_EPADDR,
			.Size             = CDC_NOTIFICATION_EPSIZE,
			.Banks            = 1,
		},
	},
};

static FILE USBSerialStream;

void usb_init( void )
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~( 1 << WDRF );
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set( clock_div_1 );

	USB_Init();
	CDC_Device_CreateStream( &VirtualSerial_CDC_Interface, &USBSerialStream );
	GlobalInterruptEnable();
	
	delay_ms( 5000 );
	
	CDC_Device_SendString( &VirtualSerial_CDC_Interface, "Testing sending directly to the device\n\n" );
	CDC_Device_Flush( &VirtualSerial_CDC_Interface );
	
	debug( "Amiga keyboard - Erik H. Bakke\n" );
}

void usb_process( void )
{
	CDC_Device_ReceiveByte( &VirtualSerial_CDC_Interface );

	CDC_Device_USBTask( &VirtualSerial_CDC_Interface );
	USB_USBTask();
}

void EVENT_USB_Device_Connect( void )
{
}

void EVENT_USB_Device_Disconnect( void )
{
}

void EVENT_USB_Device_ConfigurationChanged( void )
{
	bool ConfigSuccess = true;

	ConfigSuccess &= CDC_Device_ConfigureEndpoints( &VirtualSerial_CDC_Interface );
}

void EVENT_USB_Device_ControlRequest( void )
{
	CDC_Device_ProcessControlRequest( &VirtualSerial_CDC_Interface );
}

void EVENT_CDC_Device_ControLineStateChanged( USB_ClassInfo_CDC_Device_t *const CDCInterfaceInfo )
{
	/* You can get changes to the virtual CDC lines in this callback; a common
	   use-case is to use the Data Terminal Ready (DTR) flag to enable and
	   disable CDC communications in your application when set to avoid the
	   application blocking while waiting for a host to become ready and read
	   in the pending data from the USB endpoints.
	*/
	bool HostReady = ( CDCInterfaceInfo->State.ControlLineStates.HostToDevice & CDC_CONTROL_LINE_OUT_DTR ) != 0;
}
