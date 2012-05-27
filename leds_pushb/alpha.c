/*****************************************************************************
 * LED_Test.c
 *****************************************************************************/

#include <cdefBF537.h>

#define _DELAY 25600000

void delay(int d)
{
	int i;
	for (i = 0; i < d; i++);
}

//Initialize LEDs and Pushbuttons
void init( void )
{
	/*
		GPIO Direction Registers (PORTxIO_DIR)
		For all bits, 0 - Input, 1 - Output
		Set as output PF6 - PF11 (LED1 - LED6)
	*/
	*pPORTFIO_DIR |= (0x003F << 6);
	
	/*
		GPIO Input Enable Registers (PORTxIO_INEN)
		For all bits, 0 - Input Buffer Disabled, 1 - Input Buffer Enabled
		Input enabled for PF2 - PF5 (SW10 - SW13)
	*/
	*pPORTFIO_INEN |= (0x000F << 2);
}

//Set the status of the 6 LEDs
void led_set(int data)
{
	/*
		GPIO Clear Registers (PORTxIO_CLEAR)
		Write-1-to-clear
	*/
	*pPORTFIO_CLEAR |= (0x003F << 6);
	
	/*
		GPIO Set Registers (PORTxIO_SET)
		Write-1-to-set
	*/
	*pPORTFIO_SET |= (data << 6);
}

//Read the 4 Pushbuttons
int pb_read( void )
{
	int buttons;
	
	//We are interested in only PF2 - PF5 (4 bits)
	buttons = (*pPORTFIO >> 2) & 0x0F;
	return buttons;
}

int main( void )
{
	int btnState, btnPrev = 0;
	int state = 0;
	int leds = 0x01;
	
	init();
	
	while (1)
	{	
		led_set(leds);
		
		//Read button PB1 - SW13
		btnState = (pb_read() & 0x01) > 0;
		
		/*
			State machine
			If button is pressed and wasn't pressed before
			(Rising edge)
		*/
		if (btnState & (btnPrev == 0))
		{
			state = ~state;	//Toggle the state
		}
		btnPrev = btnState;	//Previous state is now Current state (for the next cycle)
		
		/*
			The LEDs will flash in a direction depending on the "state" variable
		*/
		if (!state)
		{
			leds = leds << 1;
			if (leds > 0x20) leds = 0x01;
		}
		else
		{
			leds = leds >> 1;
			if (leds < 0x01) leds = 0x20;
		}
		
		delay(_DELAY);
	}
	
}
