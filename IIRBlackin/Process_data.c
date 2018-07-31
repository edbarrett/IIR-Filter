#include "Talkthrough.h"
#include "stdfix.h"

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
void Process_Data(void)
{
	//a is already multiplied by -1 after a0
	static const fract a1[3] = {0.250000000000000r,  0.443619894003739r,   -0.243665008994394r}; //manually changed after a0
	static const fract a2[3] = {0.250000000000000r,  0.453415590656371r,   -0.244246271582135r}; //manually changed after a0
	static const fract b1[3] = {0.250000000000000r, -0.500000000000000r, 0.250000000000000r}; 
	static const fract b2[3] = {0.250000000000000r, 0.500000000000000r, 0.250000000000000r};
	
	 
	static fract x[3] = {0.0r};
	static fract y1[3] = {0.0r}; 
	static fract y2[3] = {0.0r};
	int j;

	//gain
	fract g = 0.030855774600859r;
	//first segment
	accum y1input;
	//second segment
	accum y2input;
	
	x[0] = rbits((short)(iChannel0LeftIn>>16)); 
	
	/////////////////////////////////////////////
	//Here begins the first Direct Form 1 segment
	/////////////////////////////////////////////
	
	
	y1input = (x[0]*(b1[0])*g+x[1]*(b1[1])*g+x[2]*(b1[2])*g+y1[1]*(a1[1])+y1[2]*(a1[2]))*4;
	
	//Converting to fract
	if(y1input >   FRACT_MAX){
		y1input =   FRACT_MAX;
	}if(y1input < FRACT_MIN){
		y1input = FRACT_MIN;
	}
	y1[0] = (fract)y1input;  //output from the first second order segment
	
	/////////////////////////////////////////////
	//Here begins the second Direct Form 1 segment
	/////////////////////////////////////////////
	
	
	y2input = (y1[0]*(b2[0])*g+y1[1]*(b2[1])*g+y1[2]*(b2[2])*g+y2[1]*(a2[1])+y2[2]*(a2[2]))*4;
	
	//Converting to fract
	if(y2input >   FRACT_MAX){
		y2input =   FRACT_MAX;
	}if(y2input < FRACT_MIN){
		y2input = FRACT_MIN;
	}
	y2[0] = (fract)y2input;  //output from the first second order segment
	
	
	
	////shifting the delay line
	for(j=3;j>=1;j--){		
		x[j]=x[j-1];
		y1[j] = y1[j-1];
		y2[j] = y2[j-1];
	}
	
	iChannel0LeftOut = bitslr((long fract)y2input); //output
	iChannel0RightOut = iChannel0LeftIn; //copy input to right channel
	iChannel1LeftOut = iChannel1LeftIn;
	iChannel1RightOut = iChannel1RightIn;
}