#include <stdlib.h>
#include <stdio.h>
#include <string.h>



enum  SAL_FILE_TYPE_ENUM
{
	SAL_FILE_TYPE_FILE = 0,
	SAL_FILE_TYPE_DIRECTORY
};

struct SAL_DIRECTORY_ENTRY
{
	s8 filename[SAL_MAX_PATH];
	s8 displayName[SAL_MAX_PATH];
	s32 type;
};

s32 sal_Init(void);
s8 *sal_LastErrorGet();
void sal_LastErrorSet(char *msg);
s32 sal_StringCompare(const char *string1, const char *string2);

u32 sal_VideoInit(u32 bpp, u32 color, u32 refreshRate);
u32 sal_VideoGetBpp();
void sal_VideoClear(u32 color);
void sal_VideoClearAll(u32 color);
void sal_VideoDrawRect(s32 x, s32 y, s32 width, s32 height, u32 color);
void sal_VideoPrint(s32 x, s32 y, const char *buffer, u32 color);
void sal_VideoFlip(s32 vsync);
void *sal_VideoGetBuffer();
u32 sal_VideoSetScaling(s32 width, s32 height);
void sal_VideoPaletteSet(u32 index, u32 color);
void sal_VideoPaletteSync();
void sal_VideoBitmapScale(int startx, int starty, int viswidth, int visheight, int newwidth, int newheight,int pitch, u16 *src, u16 *dst);

s32 sal_AudioInit(s32 rate, s32 bits, s32 stereo, s32 Hz);
void sal_AudioClose(void);
u32 sal_AudioGetCurrentBufferIndex();
u32 sal_AudioGetPrevBufferIndex();
u32 sal_AudioGetNextBufferIndex(u32 index);
s16 *sal_AudioGetBuffer(u32 bufferIndex);

u32 sal_AudioGetBufferSize();
u32 sal_AudioGetSampleCount();
void sal_AudioSetVolume(s32 l, s32 r);
u32 sal_AudioRatePrevious(u32 currRate);
u32 sal_AudioRateNext(u32 currRate);

void sal_CpuSpeedSet(u32 mhz);
u32 sal_CpuSpeedPrevious(u32 currSpeed);
u32 sal_CpuSpeedNext(u32 currSpeed);
u32 sal_CpuSpeedPreviousFast(u32 currSpeed);
u32 sal_CpuSpeedNextFast(u32 currSpeed);

void sal_Reset(void);
s32 sal_TimerInit(s32 freq);
void sal_TimerClose();
u32 sal_TimerRead();

u32 sal_InputRepeat();
u32 sal_InputHeld();
void sal_InputWaitForRelease();
void sal_InputWaitForPress();
void sal_InputIgnore();
u32 sal_InputPoll();
u32 sal_InputPollRepeat();

void sal_Sleep(u32 milliSecs);

s32 sal_ZipLoad(const char *filename, s8 *buffer, s32 bufferMaxSize, s32 *file_size);
s32 sal_ZipSave(const char *filename, s8 *firstFilename, s8 *buffer, s32 size);
s32 sal_ZipGetFirstCrc(const char *filename, s32 *crc);
void sal_ZipGetFirstFilename(const char *filename, s8 *longfilename);
s32 sal_ZipCheck(const char *filename);

s32 sal_FileLoad(const char *filename, u8 *buffer, u32 maxsize, u32 *filesize);
s32 sal_FileSave(const char *filename, u8 *buffer, u32 bufferSize);
s32 sal_FileDelete(const char *filename);
s32 sal_FileExists(const char *filename);
s32 sal_FileGetSize(const char *filename, u32 *filesize);
u32 sal_FileGetCRC(u8 *data, u32 size);

const char * sal_DirectoryGetHome(void);
void sal_DirectorySplitFilename(const char *wholeFilename, s8* path, s8 *filename, s8 *ext);
void sal_DirectoryGetParent(s8 *path);
s32 sal_DirectoryGetCurrent(s8 *path, u32 size);
void sal_DirectoryCombine(s8 *path, const char *name);
s32 sal_DirectoryGetItemCount(const char *path, s32 *returnItemCount);
s32 sal_DirectoryGet(const char *path, struct SAL_DIRECTORY_ENTRY *dir,
			s32 startIndex, s32 count);
s32 sal_DirectoryCreate(const char *path);
s32 sal_DirectoryOpen(const char *path, struct SAL_DIR *d);
s32 sal_DirectoryClose(struct SAL_DIR *d);
s32 sal_DirectoryRead(struct SAL_DIR *d, struct SAL_DIRECTORY_ENTRY *dir);

s32 sal_ImageLoad(const char *fname, void *dest, u32 width, u32 height);
s32 sal_ImageDrawTiled(u16 *image, u32 width, u32 height, s32 xScroll, s32 yScroll, s32 x, s32 y);
s32 sal_ImageDraw(u16 *image, u32 width, u32 height, s32 x, s32 y);

