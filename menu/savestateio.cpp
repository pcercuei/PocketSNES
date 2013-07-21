
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sal.h"
#include "savestateio.h"
#include "zip.h"
#include "unzip.h"

// save state file I/O
static int mIOMode = 0;


static FILE  *mFile = 0;
static char mFileName[SAL_MAX_PATH];
static char *mFileMem = NULL;
static int mFileMemPos = 0;
static int mFileMemSize=0;
static int mFileRWMode = 0;
static int mFileZipMode = 0;


static u8 **mMem = 0;
static int mMemSize = 0;
static int mMemPos = 0;
static int mMemRWMode = 0;

static
int check_zip(char *filename)
{
    u8 buf[2];
    FILE *fd = NULL;
    fd = (FILE*)fopen(filename, "rb");
    if(!fd) return (0);
    fread(buf, 1, 2, fd);
    fclose(fd);
    if(memcmp(buf, "PK", 2) == 0) return (1);
    return (0);
}

static char *load_archive(char *filename, int *file_size)
{
    int size = 0;
    char *buf = NULL;	

    unzFile fd = NULL;
    unz_file_info info;
    int ret = 0;
         
	/* Attempt to open the archive */
	fd = unzOpen(filename);
	if(!fd)
	{
		printf("Failed to open archive\r\n");
		return NULL;
	}

	/* Go to first file in archive */
	ret = unzGoToFirstFile(fd);
	if(ret != UNZ_OK)
	{
		printf("Failed to find first file in zip\r\n");
		unzClose(fd);
		return NULL;
	}

	ret = unzGetCurrentFileInfo(fd, &info, NULL, 0, NULL, 0, NULL, 0);
	if(ret != UNZ_OK)
	{
		printf("Failed to zip info\r\n");
        unzClose(fd);
        return NULL;
	}

	/* Open the file for reading */
	ret = unzOpenCurrentFile(fd);
	if(ret != UNZ_OK)
	{
	    printf("Failed to read file\r\n");
		unzClose(fd);
		return NULL;
	}

	/* Allocate file data buffer */
	size = info.uncompressed_size;
	buf=(char*)malloc(size);
	if(!buf)
	{
		printf("Failed to malloc zip buffer\r\n");
		unzClose(fd);
		return NULL;
	}
	
	/* Read (decompress) the file */
	ret = unzReadCurrentFile(fd, buf, info.uncompressed_size);
	if(ret != info.uncompressed_size)
	{
		free(buf);
	    printf("File failed to uncompress fully\r\n");
	    unzCloseCurrentFile(fd);
		unzClose(fd);
		return NULL;
	}

	/* Close the current file */
	ret = unzCloseCurrentFile(fd);
	if(ret != UNZ_OK)
	{
		free(buf);
	    printf("Failed to close file in zip\r\n");
	    unzClose(fd);
		return NULL;
	}

	/* Close the archive */
	ret = unzClose(fd);
	if(ret != UNZ_OK)
	{
		free(buf);
	    printf("Failed to close zip\r\n");
	    return NULL;
	}

	/* Update file size and return pointer to file data */
	*file_size = size;
	return buf;
}

static int save_archive(char *filename, char *buffer, int size)
{	
    zipFile fd = NULL;
    int ret = 0;
    fd=zipOpen(filename,0);
    if(!fd)
    {
       printf("Failed to create zip\r\n");
       return (0);
    }

    ret=zipOpenNewFileInZip(fd,"SNAPSHOT",
			    NULL,
				NULL,0,
			    NULL,0,
			    NULL,
			    Z_DEFLATED,
			    Z_BEST_COMPRESSION);
			    
    if(ret != ZIP_OK)
    {
       zipClose(fd,NULL);
       printf("Failed to create file in zip\r\n");
       return (0);    
    }

    ret=zipWriteInFileInZip(fd,buffer,size);
    if(ret != ZIP_OK)
    {
      zipCloseFileInZip(fd);
      zipClose(fd,NULL);
      printf("Failed to write file in zip\r\n");
      return (0);
    }

    ret=zipCloseFileInZip(fd);
    if(ret != ZIP_OK)
    {
      zipClose(fd,NULL);
      printf("Failed to close file in zip\r\n");
      return (0);
    }

    ret=zipClose(fd,NULL);
    if(ret != ZIP_OK)
    {
      printf("Failed to close zip\r\n");
      return (0);
    }
	
    return(1);
}

static
int state_unc_open(const char *fname, const char *mode)
{
	//mode = "wb"  or "rb"
	//If mode is write then create a new buffer to hold written data
	//when file is closed buffer will be compressed to zip file and then freed
	if(mode[0]=='r')
	{
		//Read mode requested
		if(check_zip((char*)fname))
		{
			//File is a zip, so uncompress
			mFileZipMode = 1; //zip mode
			mFileRWMode = 0;
			mFileMem=load_archive((char*)fname,&mFileMemSize);
			if(!mFileMem) return 0;
			mFileMemPos=0;
			strcpy(mFileName,fname);
			return 1;
		}
		else
		{
			mFileZipMode = 0; //normal file mode
			mFile = fopen(fname, mode);
			return (int) mFile;
		}
	}
	else
	{
		//Write mode requested. Zip only option
		mFileRWMode = 1;
		mFileZipMode = 1; //always zip
		mFileMem=(char*)malloc(200);
		mFileMemSize=200;
		mFileMemPos = 0;
		strcpy(mFileName,fname);
		return 1;
	}
}

static
int state_unc_read(void *p, int l)
{
	if(mFileZipMode==0)
	{
		return fread(p, 1, l, mFile);
	}
	else
	{
		
		if((mFileMemPos+l)>mFileMemSize)
		{
			//Read requested that exceeded memory limits
			return 0;
		}
		else
		{
			memcpy(p,mFileMem+mFileMemPos,l);
			mFileMemPos+=l;
		}
		return l;
	}
}

static
int state_unc_write(void *p, int l)
{
	if(mFileZipMode==0)
	{
		return fwrite(p, 1, l, mFile);
	}
	else
	{
		if((mFileMemPos+l)>mFileMemSize)
		{
			//Write will exceed current buffer, re-alloc buffer and continue
			mFileMem=(char*)realloc(mFileMem,mFileMemPos+l);
			mFileMemSize=mFileMemPos+l;
		}
		//Now do write
		memcpy(mFileMem+mFileMemPos,p,l);
		mFileMemPos+=l;
		return l;
	}
}

static
void state_unc_close()
{
	if(mFileZipMode==0)
	{
		fclose(mFile);
	}
	else
	{
		if (mFileRWMode == 1)
			save_archive(mFileName,mFileMem,mFileMemSize);

		free(mFileMem);
		mFileMem=NULL;
		mFileMemSize=0;
		mFileMemPos=0;
		mFileName[0]=0;
	}
}

static
int state_mem_open(const char *fname, const char *mode)
{
	//Streams data to or from a memory buffer
	//when opening code assumes memory buffer already
	//exists, so do not try loading before you save...it aint gonna work!
	if(mode[0]=='r')
	{
		mMemRWMode=0;
		//state_mem - assume this has been set by previous W open
		if(!*mMem) return 0;
		mMemPos=0;
		return 1;
	}
	else
	{
		//Write mode requested.
		mMemRWMode = 1;
		if(*mMem) free(*mMem);
		*mMem=NULL;
		*mMem=(u8*)malloc(200);
		mMemSize=200;
		mMemPos = 0;
		return 1;
	}
}

static
int state_mem_read(void *p, int l)
{

	if((mMemPos+l)>mMemSize)
	{
		//Read requested that exceeded memory limits
		return 0;
	}
	else
	{
		memcpy(p,*mMem+mMemPos,l);
		mMemPos+=l;
	}
	return l;
}

static
int state_mem_write(void *p, int l)
{
	if((mMemPos+l)>mMemSize)
	{
		//Write will exceed current buffer, re-alloc buffer and continue
		*mMem=(u8*)realloc(*mMem,mMemPos+l);
		mMemSize=mMemPos+l;
	}
	//Now do write
	memcpy(*mMem+mMemPos,p,l);
	mMemPos+=l;
	return l;
}

static
void state_mem_close()
{
	//Let caller free memory - yeah yuck I know
	mMemPos=0;
}



#define SAVE_STATE_IO_MODE_MEMORY 1
#define SAVE_STATE_IO_MODE_FILE   0

void SetSaveStateIoModeMemory(u8 **data)
{
	mIOMode=SAVE_STATE_IO_MODE_MEMORY;
	mMem=data;
}

void SetSaveStateIoModeFile()
{
	mIOMode=SAVE_STATE_IO_MODE_FILE;
}

int  statef_open(const char *fname, const char *mode)
{
	if(mIOMode==SAVE_STATE_IO_MODE_MEMORY) return state_mem_open(fname,mode);
	else	return state_unc_open(fname,mode);
}

int  statef_read(void *p, int l)
{
	if(mIOMode==SAVE_STATE_IO_MODE_MEMORY) return state_mem_read(p,l);
	else	return state_unc_read(p,l);
}

int  statef_write(void *p, int l)
{
	if(mIOMode==SAVE_STATE_IO_MODE_MEMORY) return state_mem_write(p,l);
	else	return state_unc_write(p,l);
}

void statef_close()
{
	if(mIOMode==SAVE_STATE_IO_MODE_MEMORY) return state_mem_close();
	else	return state_unc_close();
}


