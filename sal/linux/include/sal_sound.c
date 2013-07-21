#include <stdlib.h>
#include <SDL.h>

static SDL_AudioSpec audiospec;
static unsigned int buf_w, buf_r;

static void sdl_audio_callback (void *userdata, Uint8 *stream, int len)
{
	memcpy(stream, mSoundBuffer[buf_r], len);
	if (++buf_r >= SOUND_BUFFER_COUNT) 
		buf_r = 0;
}

void sal_SubmitSamples(void *buff, int len)
{
    SDL_LockAudio();
	memcpy(mSoundBuffer[buf_w], buff, len);

	if (buf_w != buf_r && ++buf_w == SOUND_BUFFER_COUNT)
		buf_w = 0;

	SDL_UnlockAudio();
}

s32 sal_AudioInit(s32 rate, s32 bits, s32 stereo, s32 Hz)
{
	audiospec.freq = rate;
	audiospec.channels = (stereo + 1);
	audiospec.format = AUDIO_S16;

	audiospec.samples = (rate / Hz);
	if (!stereo && audiospec.samples & 1)
		audiospec.samples--;

	mSoundSampleCount = audiospec.samples * audiospec.channels;
	mSoundBufferSize = mSoundSampleCount * (bits >> 3);

	audiospec.callback = sdl_audio_callback;

	if (SDL_OpenAudio(&audiospec, NULL) < 0) {
		fprintf(stderr, "Unable to initialize audio.\n");
		return SAL_ERROR;
	}

	SDL_PauseAudio(0);
	return SAL_OK;
}

void sal_AudioClose(void)
{
	SDL_PauseAudio(1);
	SDL_CloseAudio();
}

void sal_AudioSetVolume(s32 l, s32 r) 
{ 
} 
