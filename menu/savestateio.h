#ifndef _SAVESTATEIO_H_
#define _SAVESTATEIO_H_

void SetSaveStateIoModeMemory(unsigned char **data);
void SetSaveStateIoModeFile();

int  statef_open(const char *fname, const char *mode);
int  statef_read(void *p, int l);
int  statef_write(void *p, int l);
void statef_close();


#endif /* _SAVESTATEIO_H_ */





