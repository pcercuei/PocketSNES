
#include <sml/mtaudio.h>

static u32 mStereo;
static volatile u32 mVolume=15;
static u32 mSoundRate;

static void SoundThread() 
{
	mtaudio_buffer_set(mSoundBuffer[mSb], mSoundBufferSize, mStereo, mVolume, mSoundRate);

	if(mSb+1>=SOUND_BUFFER_COUNT) mSb=0;
	else mSb++;
}

s32 sal_AudioInit(s32 rate, s32 bits, s32 stereo, s32 Hz)
{
	s32 i,j;

	mSoundRate=rate;
	mStereo=stereo+1;
	mSoundSampleCount=(rate/Hz) * mStereo;
	mSoundBufferSize=mSoundSampleCount*((bits==16)?2:1);

	//Clear all sound banks
	for(i=0;i<SOUND_BUFFER_COUNT;i++)
	{
		for(j=0;j<mSoundSampleCount;j++)
		{
			mSoundBuffer[i][j] = 0;
		}
	}

	mtaudio_init(SoundThread,mSoundRate);

   	return SAL_OK;
}


void sal_AudioClose(void)
{
	mtaudio_term();
}

void sal_AudioSetVolume(s32 l, s32 r) 
{ 
	mVolume=l;
} 

