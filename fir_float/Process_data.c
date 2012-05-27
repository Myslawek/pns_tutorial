#include "Talkthrough.h"
#include "fdacoefs.h"

#define N 64

//--------------------------------------------------------------------------//
// Function:	Process_Data()												//
//																			//
// Description: This function is called from inside the SPORT0 ISR every 	//
//				time a complete audio frame has been received. The new 		//
//				input samples can be found in the variables iChannel0LeftIn,//
//				iChannel0RightIn, iChannel1LeftIn and iChannel1RightIn 		//
//				respectively. The processed	data should be stored in 		//
//				iChannel0LeftOut, iChannel0RightOut, iChannel1LeftOut,		//
//				iChannel1RightOut, iChannel2LeftOut and	iChannel2RightOut	//
//				respectively.												//
//--------------------------------------------------------------------------//

/*
	Time Domain Convolution
	Floating Point Coefficients
	
	Sampling frequency = 48 kHz
*/

int k, p = 0, index;

int data_left, data_right;

float buff_left[N];
float buff_right[N];

float l, r;

void Process_Data(void)
{
	data_left = ((iChannel0LeftIn<<8)>>0)>>8;
	data_right = ((iChannel0RightIn<<8)>>0)>>8;
	
	buff_left[p] = data_left;
	buff_right[p] = data_right;
	p = (p + 1) % N;
	
	//if button is pressed, output the unfiltered audio samples
	if (*pPORTFIO & (1 << 2))
	{
		iChannel0LeftOut = data_left;
		iChannel0RightOut = data_right;
		return;
	}
	
	l = 0;
	r = 0;
	index = p;	//index points at the oldest element in buffer
	
	for (k = 0; k < N; k++)
	{
		l += buff_left[index] * filter[N - k - 1];
		r += buff_right[index] * filter[N - k - 1];
		
		index = (index + 1) % N;
	}
	
	iChannel0LeftOut = l;
	iChannel0RightOut = r;
}
