
#include "sal.h"

int mainEntry(int argc, char *argv[])
{
	u16 col=0;
	u32 keys=0;
	sal_Init();
	sal_VideoInit(16,0,60);
	while(1)
	{
		sal_VideoClear(col);
		sal_VideoPrint(0,0,"testing",0xFFFF);
		sal_VideoFlip(1);
		col++;
		keys=sal_InputPoll();
		if(keys&SAL_INPUT_START) break;
	}	
	
	sal_Reset();
	
	return 0;
}



