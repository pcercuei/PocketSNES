
#include <stdio.h>
#include <png/png.h>
#include <dingoo/ucos2.h>
#include <dingoo/keyboard.h>
#include <dingoo/jz4740.h>
#include <jz4740/cpu.h>
#include <dirent.h>
#include "sal.h"
#include "unzip.h"
#include "zip.h"

#define SAL_FRAME_BUFFER_COUNT	4
#define SOUND_BUFFER_COUNT 	8
#define AUDIO_RATE_COUNT	5
#define MAX_SOUND_LEN 	((48000/60)*2)


static void *mFbMem[4]={0,0,0,0};
static u32 mSoundSampleCount=0;
static u32 mSoundBufferSize=0;
static u32 mSoundThreadFlag=0;
static u32 mSoundLastCpuSpeed=0;
static u32 mPaletteBuffer[0x100];
static u32 mInputFirst=0;
static s8  mCurrDir[SAL_MAX_PATH];

s32 mAudioRateLookup[AUDIO_RATE_COUNT]={8250,11025,22050,44100,48000};

#include "sal_common.c"
#include "sal_sound.c"
#include "sal_filesys.c"
#include "sal_timer.c"
#include "sal_video.c"

u32 sal_InputPoll()
{
	int i=0;
	u32 inputHeld=0;
	u32 timer=0;

  	inputHeld = (u32)kbd_get_key();

	//clean value
	inputHeld&=(SAL_INPUT_UP|SAL_INPUT_DOWN|SAL_INPUT_LEFT|SAL_INPUT_RIGHT|SAL_INPUT_A|
				SAL_INPUT_B	|SAL_INPUT_X|SAL_INPUT_Y|SAL_INPUT_L|SAL_INPUT_R|SAL_INPUT_START|
				SAL_INPUT_SELECT|SAL_INPUT_POWER);
	// Process key repeats
	timer=sal_TimerRead();
	for (i=0;i<32;i++)
	{
		if (inputHeld&(1<<i)) 
		{
			if(mInputFirst&(1<<i))
			{
				if (mInputRepeatTimer[i]<timer)
				{
					mInputRepeat|=1<<i;
					mInputRepeatTimer[i]=timer+10;
				}
				else
				{
					mInputRepeat&=~(1<<i);
				}
			}
			else
			{
				//First press of button
				//set timer to expire later than usual
				mInputFirst|=(1<<i);
				mInputRepeat|=1<<i;
				mInputRepeatTimer[i]=timer+50;
			}
		}
		else			
		{
			mInputRepeatTimer[i]=timer-10;
			mInputRepeat&=~(1<<i);
			mInputFirst&=~(1<<i);
		}
		
	}

	if(mInputIgnore)
	{
		//A request to ignore all key presses until all keys have been released has been made
		//check for release and clear flag, otherwise clear inputHeld and mInputRepeat
		if (inputHeld == 0)
		{
			mInputIgnore=0;
		}
		inputHeld=0;
		mInputRepeat=0;
	}

	return inputHeld;
}

void sal_CpuSpeedSet(u32 mhz)
{
	//Screws with TV out mode so disabling cpu scaling for the moment

	uintptr_t tempCore=mhz*1000000;
	cpu_clock_set(tempCore);
}

u32 sal_CpuSpeedNext(u32 currSpeed)
{
	u32 newSpeed=currSpeed+1;
	if(newSpeed > 500) newSpeed = 500;
	return newSpeed;
}

u32 sal_CpuSpeedPrevious(u32 currSpeed)
{
	u32 newSpeed=currSpeed-1;
	if(newSpeed > 500) newSpeed = 0;
	return newSpeed;
}

u32 sal_CpuSpeedNextFast(u32 currSpeed)
{
	u32 newSpeed=currSpeed+10;
	if(newSpeed > 500) newSpeed = 500;
	return newSpeed;
}

u32 sal_CpuSpeedPreviousFast(u32 currSpeed)
{
	u32 newSpeed=currSpeed-10;
	if(newSpeed > 500) newSpeed = 0;
	return newSpeed;
}

s32 sal_Init(void)
{
	sal_TimerInit(60);
	memset(mInputRepeatTimer,0,sizeof(mInputRepeatTimer));
	return SAL_OK;
}

void sal_Reset(void)
{
	sal_AudioClose();
}

static
void GamePathInit(const char* inPath) {
	uintptr_t i, j;
	for(i = 0, j = 0; inPath[i] != '\0'; i++) {
		if((inPath[i] == '\\') || (inPath[i] == '/'))
			j = i;// + 1;
	}
	strncpy(mCurrDir, inPath, j);
	mCurrDir[j] = '\0';
    /*
    ** Static (hard coded) path examples
    strcpy(gamePath, "a:\\"); // WORKS
    strcpy(gamePath, "a:/"); // unix style path FAILS
    strcpy(gamePath, "a:\\"); // DEBUG NOTE multiple slashes which results in look up failure 
    */
}


int mainEntry(int argc, char *argv[]);

#if defined(__DINGOO_APP__)
// Prove entry point wrapper
int main(int argc, char *argv[])
{	
	//Seed random generator
	srand(OSTimeGet());

	
	GamePathInit(argv[0]); /* workout directory containing this exe and list it */
	return mainEntry(1,argv);
}
#endif

#if defined(__DINGOO_SIM__)
// Prove entry point wrapper
int main(int argc, char *argv[])
{	
	//Seed random generator
	srand(OSTimeGet());
	char *newArgs[2]={argv[0],argv[0]};
	return mainEntry(2,&newArgs[0]);
}
#endif


