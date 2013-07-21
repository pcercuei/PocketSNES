static u32 mTimerFreq=1000000/60;

u32 sal_TimerRead()
{
	return clock()/mTimerFreq;
}

s32 sal_TimerInit(s32 freq)
{
	mTimerFreq=(1000000)/freq;
   	return SAL_OK;
}

void sal_TimerClose(void) 
{

}

