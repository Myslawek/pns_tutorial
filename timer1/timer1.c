/*****************************************************************************
 * timer1.c
 	Without interrupts
 *****************************************************************************/

#include <cdefBF537.h> 

#define PERIOD 		50000000
#define OVRPIN 		23
#define TIMER_NR  	7

//Initialize LEDs
void init( void )
{
	//Set as output PF6 - PF11 (LED1 - LED6)
	*pPORTFIO_DIR |= (0x003F << 6);
}

//Toggle LEDs on and off
void led_toggle( void )
{
	*pPORTFIO_TOGGLE |= (0x003F << 6);
}

void timer_init( void )
{
	/*
		Superior Limit Timer Period
		Use system clock SCLK for counter
		SCLK = 50 MHz (default) => Period has to be 50*10^6 for 1 sec LED flash
	*/
	*pTIMER7_PERIOD = PERIOD;
	
	/*
		Configure as:
		
		PWM_OUT mode
		Count to end of Period
		Output Pad Disable
	*/
	*pTIMER7_CONFIG = 0x0049;
	
	//Enable Timer nr. 7
	*pTIMER_ENABLE |= (1 << TIMER_NR);
}

//Test if and overflow occured for Timer 7
int timer_ovr_check( void )
{
	//TOVF_ERR7 (Timer7 Counter Overflow)
	//Indicates that an error or an overflow occurred
	return (*pTIMER_STATUS & (1 << OVRPIN));
}

//Reset the overflow for Timer 7 by writing a '1'
void timer_ovr_reset( void )
{
	*pTIMER_STATUS |= (1 << OVRPIN);
}

int main( void )
{
	init();
	timer_init();
	
	while (1)
	{	
		if (timer_ovr_check())
		{
			timer_ovr_reset();
			led_toggle();
		}
	}	
}
