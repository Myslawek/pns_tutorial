#include "Talkthrough.h"

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
	32 bits variables
	24 bits samples, aligned to the right
*/

#define	N 96000	//Buffer Size
#define AT 1	//Attenuation of echo

int buff_left[N];
int buff_right[N];

int wr_ptr = 0;
int rd_ptr = 1;

int data_left;
int data_right;

void Process_Data(void)
{
	//Sign extension in 2's complement
	data_left = ((iChannel0LeftIn<<8)>> AT )>>8;
	data_right = ((iChannel0RightIn<<8)>> AT )>>8;
	
	buff_left[wr_ptr] = data_left;
	buff_right[wr_ptr] = data_right;
	
	// * 3 / 4 (for further attenuation of the entire signal)
	iChannel0LeftOut = (data_left + buff_left[rd_ptr]) * 3 / 4;
	iChannel0RightOut = (data_right + buff_right[rd_ptr]) * 3 / 4;
	
	wr_ptr = (wr_ptr + 1) % N;	//Update write pointer
	rd_ptr = (rd_ptr + 1) % N;	//Update read pointer
}
