#include "VGMRtos.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef VGM_NO_ZLIB

gzFile gzopen(const char *path, const char *mode)
{
    gzFile f = malloc( sizeof(gzFile_s) );
    vgm_progmem_t *fileinfo = (vgm_progmem_t *)path;
    f->data = fileinfo->data;
    f->size = fileinfo->size;
    f->position = 0;
    return f;
}

int gzread(gzFile file, voidp buf, unsigned len)
{
    int size = file->size - file->position;
    if ( len < size )
    {
        size = len;
    }
    memcpy(buf, file->data + file->position, size);
    file->position += size;
    return size;
}

int gzclose(gzFile file)
{
    if ( file )
    {
        free((void *)file);
    }
    return Z_OK;
}

int gzrewind(gzFile file)
{
    file->position = 0;
    return Z_OK;
}

int gzseek(gzFile file, int offset, int whence)
{
    if ( whence == SEEK_SET )
    {
        file->position = 0;
    }
    return file->position;
}

int gztell(gzFile file)
{
    return file->position;
}

#endif

#ifdef VGM_RTOS

#include "chips/mamedef.h"
#include "stdbool.h"
#include "VGMPlay.h"
#include "VGMPlay_Intf.h"

#define SAMPLESIZE sizeof(WAVE_16BS)

#ifdef CONSOLE_MODE
UINT8 CmdList[0x100]; // used by VGMPlay.c and VGMPlay_AddFmts.c
bool ErrorHappened;   // used by VGMPlay.c and VGMPlay_AddFmts.c
#endif
extern VGM_HEADER VGMHead;
extern UINT32 SampleRate;
extern float VolumeLevel;
extern UINT32 VGMMaxLoopM;
extern UINT32 FadeTime;
extern bool EndPlay;
extern char *AppPaths[8];

int vgm_set_format(uint32_t frequency)
{
	SampleRate = frequency;
	return 0;
}

int vgm_set_volume(float volume)
{
	VolumeLevel = volume;
	return 0;
}

int vgm_play_start(const uint8_t *data, int size)
{
	static vgm_progmem_t fileinfo = {};
	fileinfo.data = data;
	fileinfo.size = size;
	VGMPlay_Init();
	VGMPlay_Init2();
	if (!OpenVGMFile((const char *)&fileinfo))
	{
		fprintf(stderr, "vgm2pcm: error: failed to open vgm_file\n");
		return 1;
	}
	PlayVGM();
	return 0;
}

int vgm_play_data(void *outBuffer, int size_in_bytes)
{
	UINT32 maxSamples = size_in_bytes / SAMPLESIZE;

	if (EndPlay)
	{
		return 0;
	}

	UINT32 samples = FillBuffer(outBuffer, maxSamples);
	return samples * SAMPLESIZE;
}

int vgm_play_stop(void)
{
	StopVGM();

	CloseVGMFile();

	VGMPlay_Deinit();

	return 0;
}

#endif
