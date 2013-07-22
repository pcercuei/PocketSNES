
#ifndef _SAL_H_
#define _SAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <dirent.h>

typedef uint32_t u32;
typedef int32_t  s32;
typedef uint16_t u16;
typedef int16_t  s16;
typedef uint8_t  u8;
typedef char     s8;

#ifdef __DINGUX__
#define SAL_INPUT_INDEX_UP			20
#define SAL_INPUT_INDEX_DOWN			27
#define SAL_INPUT_INDEX_LEFT			28
#define SAL_INPUT_INDEX_RIGHT			18
#define SAL_INPUT_INDEX_A			31
#define SAL_INPUT_INDEX_B			21
#define SAL_INPUT_INDEX_X			16
#define SAL_INPUT_INDEX_Y			6
#define SAL_INPUT_INDEX_L			8
#define SAL_INPUT_INDEX_R			29
#define SAL_INPUT_INDEX_START			11
#define SAL_INPUT_INDEX_SELECT			10
#define SAL_INPUT_INDEX_VOL_UP			0
#define SAL_INPUT_INDEX_VOL_DOWN		0
#define SAL_INPUT_INDEX_STICK_PUSH		0
#define SAL_INPUT_INDEX_POWER			7
#else
#define SAL_INPUT_INDEX_UP			0
#define SAL_INPUT_INDEX_DOWN			1
#define SAL_INPUT_INDEX_LEFT			2
#define SAL_INPUT_INDEX_RIGHT			3
#define SAL_INPUT_INDEX_A			4
#define SAL_INPUT_INDEX_B			5
#define SAL_INPUT_INDEX_X			6
#define SAL_INPUT_INDEX_Y			7
#define SAL_INPUT_INDEX_L			8
#define SAL_INPUT_INDEX_R			9
#define SAL_INPUT_INDEX_START			10
#define SAL_INPUT_INDEX_SELECT			11
#define SAL_INPUT_INDEX_VOL_UP			12
#define SAL_INPUT_INDEX_VOL_DOWN		13
#define SAL_INPUT_INDEX_STICK_PUSH		14
#endif

#define SAL_INPUT_UP			(1<<SAL_INPUT_INDEX_UP)
#define SAL_INPUT_DOWN			(1<<SAL_INPUT_INDEX_DOWN)
#define SAL_INPUT_LEFT			(1<<SAL_INPUT_INDEX_LEFT)
#define SAL_INPUT_RIGHT			(1<<SAL_INPUT_INDEX_RIGHT)
#define SAL_INPUT_A			(1<<SAL_INPUT_INDEX_A)
#define SAL_INPUT_B			(1<<SAL_INPUT_INDEX_B)
#define SAL_INPUT_X			(1<<SAL_INPUT_INDEX_X)
#define SAL_INPUT_Y			(1<<SAL_INPUT_INDEX_Y)
#define SAL_INPUT_L			(1<<SAL_INPUT_INDEX_L)
#define SAL_INPUT_R			(1<<SAL_INPUT_INDEX_R)
#define SAL_INPUT_START			(1<<SAL_INPUT_INDEX_START)
#define SAL_INPUT_SELECT		(1<<SAL_INPUT_INDEX_SELECT)
#define SAL_INPUT_VOL_UP		(1<<SAL_INPUT_INDEX_VOL_UP)
#define SAL_INPUT_VOL_DOWN		(1<<SAL_INPUT_INDEX_VOL_DOWN)
#define SAL_INPUT_STICK_PUSH		(1<<SAL_INPUT_INDEX_STICK_PUSH)

#define SAL_SCREEN_WIDTH			320
#define SAL_SCREEN_HEIGHT			240
#define SAL_SCREEN_X_STRIDE_LEFT		(0-1)
#define SAL_SCREEN_X_STRIDE_RIGHT		(1)
#define SAL_SCREEN_Y_STRIDE_UP			(0-SAL_SCREEN_WIDTH)
#define SAL_SCREEN_Y_STRIDE_DOWN		(SAL_SCREEN_WIDTH)
#define SAL_SCREEN_ROTATED			0
#define SAL_MAX_PATH				256
#define SAL_DIR_SEP				"/"
#define SAL_DIR_SEP_BAD				"\\"
#define SAL_OK						1
#define SAL_ERROR					0
#define SAL_TRUE					1
#define SAL_FALSE					0

#define SAL_RGB(r,g,b) (u16)((r) << 11 | (g) << 6 | (b) << 1 )
#define SAL_RGB_PAL(r,g,b) SAL_RGB(r,g,b)

#define SAL_SEEK_END	SEEK_END
#define SAL_SEEK_SET	SEEK_SET

struct SAL_DIR
{
	DIR *dir;
};

u32 sal_InputWait();
void sal_SubmitSamples(void);
void *sal_GetCurrentAudioBuffer(void);

#include "sal_common.h"

#ifdef __cplusplus
}
#endif

#endif /* _SAL_H_ */

