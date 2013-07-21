
#ifndef _SAL_H_
#define _SAL_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int u32;
typedef unsigned short int u16;
typedef unsigned char u8;
typedef int s32;
typedef short int s16;
typedef char s8;



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
#define SAL_INPUT_POWER			(1<<SAL_INPUT_INDEX_POWER)

#define SAL_SCREEN_WIDTH			320
#define SAL_SCREEN_HEIGHT			240
#define SAL_SCREEN_X_STRIDE_LEFT		(-1)
#define SAL_SCREEN_X_STRIDE_RIGHT		(1)
#define SAL_SCREEN_Y_STRIDE_UP			(-SAL_SCREEN_WIDTH)
#define SAL_SCREEN_Y_STRIDE_DOWN		(SAL_SCREEN_WIDTH)
#define SAL_SCREEN_ROTATED			0
#define SAL_MAX_PATH				256
#define SAL_DIR_SEP				"\\"
#define SAL_DIR_SEP_BAD				"/"	
#define SAL_OK						1
#define SAL_ERROR					0
#define SAL_TRUE					1
#define SAL_FALSE					0

#define SAL_RGB(r,g,b) (u16)((r) << 11 | (g) << 6 | (b) << 0 )
#define SAL_RGB_PAL(r,g,b) SAL_RGB(r,g,b)

#define SAL_MAX_PATH	256

struct SAL_DIR
{
	struct DIR *dir;
	u32 needParent;
	s8 *path[SAL_MAX_PATH];
	u32 drivesRead;
};

#include "sal_common.h"

#ifdef __cplusplus
}
#endif

#endif /* _SAL_H_ */





