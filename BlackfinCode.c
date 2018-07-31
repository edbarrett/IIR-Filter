void Process_Data(void)
{
	//a is already multiplied by -1 after a0
	static const fract a1[3] = {0.250000000000000r,  -0.443619894003739r,   0.243665008994394r/; //manually changed after a0
	static const fract a2[3] = {0.250000000000000r  -0.453415590656371r   0.244246271582135r}; //manually changed after a0
	static const fract b1[3] = {0.250000000000000r, -0.500000000000000r, 0.250000000000000r}; 
	static const fract b2[3] = {0.250000000000000r, 0.500000000000000r, 0.250000000000000r};
	
	 
	static fract x[3] = {0.0r};
	static fract y1[3] = {0.0r}; 
	static fract y2[3] = {0.0r};
	int j;
	fract gain = 0.000535393268822r
	accum output;
	//first segment
	long fract b01, b11, b21;
	long fract sum1;
	long fract a11, a21;
	short fract y1input;
	//second segment
	long fract b02, b12, b22;
	long fract sum2;
	long  fract a12, a22;
	short fract y2input;
	
	x[0] = rbits((short)(iChannel0LeftIn>>16)); 
	
	/////////////////////////////////////////////
	//Here begins the first Direct Form 1 segment
	/////////////////////////////////////////////
	
	//left side
	b01 = x[0]*(b1[0])*g;
	b11 = x[1]*(b1[1])*g;
	b21 = x[2]*(b1[2])*g;
	
	//middle
	sum1 = (b01+b11+b21); 
	
	//right side
	a11 = y1[1]*(a1[1]);
	a21 = y1[2]*(a1[2]);
	
	//Converting to 16 bit
	y1input = (sum1+a11+a21)*4;

	if(y1input >   FRACT_MAX){
		y1input =   FRACT_MAX;
	}if(y1input < FRACT_MIN){
		y1input = FRACT_MIN;
	}
	y1[0] = (fract)y1input;  //output from the first second order segment
	
	/////////////////////////////////////////////
	//Here begins the second Direct Form 1 segment
	/////////////////////////////////////////////
	
	//left side
	b02 = y1[0]*(b2[0]*g);
	b12 = y1[1]*(b2[1]*g);
	b22 = y1[2]*(b2[2]*g);
	
	//middle
	sum2 = (b02+b12+b22); 
	
	//right side
	a12 = y2[1]*(a2[1]);
	a22 = y2[2]*(a2[2]);
	

	//Converting to 16 bit
	y2input = (sum2+a12+a22)*4;
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