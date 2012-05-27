/*****************************************************************************
 * timer2.c
 	With interrupts
 *****************************************************************************/

#include <cdefBF537.h>
#include <sys/exception.h>

#define PERIOD 		50000000
#define OVRPIN 		23
#define TIMER_NR  	7
#define TIMER_IRQ   19

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
		IRQ Enable
	*/
	*pTIMER7_CONFIG = 0x0059;
	
	//Enable Timer nr. 7
	*pTIMER_ENABLE |= (1 << TIMER_NR);
}

//Reset the overflow for Timer 7 by writing a '1'
void timer_ovr_reset( void )
{
	*pTIMER_STATUS |= (1 << OVRPIN);
}

//Interrupt Service Routine Timer 7
EX_INTERRUPT_HANDLER(ISR_Timer7)
{
	//Clears both bit 23 (Overflow) and bit 19 (IRQ)
	timer_ovr_reset();
	
	led_toggle();
}

void interrupts_init(void)
{
	//Timer 7 Default Core Interrupt ID: 5
	*pSIC_IAR3 = 0xFFFFF5FF;
	
	//Timer 7 Interrupt (Bit 26)
	*pSIC_IMASK = 0x04000000;
	
	//IVG12 Default Mapping
	register_handler(ik_ivg12, ISR_Timer7);
}

int main( void )
{
	init();
	timer_init();
	interrupts_init();
	
	while (1)
	{
	}	
}
