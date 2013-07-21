
static u16 mFbData1[SAL_SCREEN_WIDTH*SAL_SCREEN_HEIGHT];
static u16 mFbData2[SAL_SCREEN_WIDTH*SAL_SCREEN_HEIGHT];
static u16 mFbData3[SAL_SCREEN_WIDTH*SAL_SCREEN_HEIGHT];
static u16 mFbData4[SAL_SCREEN_WIDTH*SAL_SCREEN_HEIGHT];
static u16 mFbScalingTemp[SAL_SCREEN_WIDTH*SAL_SCREEN_HEIGHT];
static u32 mVideoScaled=0;
static u32 mVideoScaleWidth=0;
static u32 mVideoScaleHeight=0;
static u32 mLastVideoFlip=0;

void SendLCDFrame(uint16_t *frame)
{
	// Wait for transfer terminated bit
	while(!(REG_DMAC_DCCSR(0)&DMAC_DCCSR_TT));

	// Enable DMA on the SLCD.
	REG_SLCD_CTRL=1;

	// Disable DMA channel while configuring.
	REG_DMAC_DCCSR(0)=0;

	// DMA request source is SLCD.
	REG_DMAC_DRSR(0)=DMAC_DRSR_RS_SLCD;

	// Set source, target and count.
	REG_DMAC_DSAR(0)=((uint32_t)frame)&0x1fffffff;
	REG_DMAC_DTAR(0)=SLCD_FIFO&0x1fffffff;
	REG_DMAC_DTCR(0)=320*240*2/16;

	// Source address increment, source width 32 bit,
	// destination width 16 bit, data unit size 16 bytes,
	// block transfer mode, no interrupt.
	REG_DMAC_DCMD(0)=DMAC_DCMD_SAI|DMAC_DCMD_SWDH_32|
	DMAC_DCMD_DWDH_16|DMAC_DCMD_DS_16BYTE|DMAC_DCMD_TM;

	__dcache_writeback_all();

	// No DMA descriptor used.
	REG_DMAC_DCCSR(0)|=DMAC_DCCSR_NDES;

	// Set enable bit to start DMA.
	REG_DMAC_DCCSR(0)|=DMAC_DCCSR_EN;
}

u32 sal_VideoInit(u32 bpp, u32 color, u32 refreshRate)
{
	mBpp=bpp;
	mRefreshRate=refreshRate;

	//mFbMem[0]=(void*)mFbData1;
	//mFbMem[1]=(void*)mFbData2;
	//mFbMem[2]=(void*)mFbData3;
	//mFbMem[3]=(void*)mFbData4;

	mFbMem[0]=(void*)_lcd_get_frame();
	mFbMem[1]=(void*)_lcd_get_frame();
	mFbMem[2]=(void*)_lcd_get_frame();
	mFbMem[3]=(void*)_lcd_get_frame();

	sal_VideoClear(color);
	mLastVideoFlip=sal_TimerRead();
	sal_VideoFlip(1);
   
	return SAL_OK;
}

void sal_VideoFlip(s32 vsync)
{
	u32 address=(u32)mFbMem[mFb];
	u32 ticks=0;
	
	if(vsync)
	{
		while(!(REG_DMAC_DCCSR(0)&DMAC_DCCSR_TT));
	}
#if 1
	if(vsync)
	{
		while(1)
		{
			ticks=sal_TimerRead();
			if(ticks-mLastVideoFlip>=1) break;
		}
		mLastVideoFlip=ticks;
	}
#endif

	//Do scaling if required
	if(mVideoScaled)
	{
		sal_VideoBitmapScale(0, 0, mVideoScaleWidth, mVideoScaleHeight, 320, 240, 0, &mFbScalingTemp[0], mFbMem[mFb]);
	}
	else
	{
		memcpy(mFbMem[mFb],&mFbScalingTemp[0],320*240*2);
	}	
	
	__dcache_writeback_all();
	_lcd_set_frame();
	//SendLCDFrame(address);

	if ((mFb+1)>=SAL_FRAME_BUFFER_COUNT) mFb=0;
	else mFb++;

	
}

u32 sal_VideoSetScaling(s32 width, s32 height)
{
	if ((width==SAL_SCREEN_WIDTH) && (height==SAL_SCREEN_HEIGHT))
	{
		mVideoScaled=0;
	}
	else
	{
		mVideoScaled=1;
	}
	mVideoScaleWidth=width;
	mVideoScaleHeight=height;
	return SAL_ERROR;
}

void *sal_VideoGetBuffer()
{
	return &mFbScalingTemp[0];
#if 0
	if (mVideoScaled)
	{
		return &mFbScalingTemp[0];
	}
	else
	{
		return mFbMem[mFb];
	}
#endif
}

void sal_VideoPaletteSync() 
{ 	
	//TODO
} 

void sal_VideoPaletteSet(u32 index, u32 color)
{
	mPaletteBuffer[index]=color;
}



