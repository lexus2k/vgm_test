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

UINT8 CmdList[0x100]; // used by VGMPlay.c and VGMPlay_AddFmts.c
bool ErrorHappened;   // used by VGMPlay.c and VGMPlay_AddFmts.c
extern VGM_HEADER VGMHead;
extern UINT32 SampleRate;
extern UINT32 VGMMaxLoopM;
extern UINT32 FadeTime;
extern bool EndPlay;
extern char *AppPaths[8];
//static char AppPathBuffer[MAX_PATH * 2];
static vgm_progmem_t fileinfo = {};

/*INLINE int fputBE16(UINT16 Value, FILE* hFile)
{
	int RetVal;
	int ResVal;

	RetVal = fputc((Value & 0xFF00) >> 8, hFile);
	RetVal = fputc((Value & 0x00FF) >> 0, hFile);
	ResVal = (RetVal != EOF) ? 0x02 : 0x00;
	return ResVal;
}*/

static WAVE_16BS *sampleBuffer;
static UINT32 maxBufferSamples;
static UINT32 bufferedSamples;

int vgm_play_start(const uint8_t *data, int size)
{
	fileinfo.data = data;
	fileinfo.size = size;
	VGMPlay_Init();
	// Path 2: exe's directory
//	AppPathPtr = AppPathBuffer;
//	AppName = GetAppFileName(); // "C:\VGMPlay\VGMPlay.exe"
	// Note: GetAppFileName always returns native directory separators.
//	if (StrPtr != NULL)
//	{
//		ChrPos = StrPtr + 1 - AppName;
//		strncpy(AppPathPtr, AppName, ChrPos);
//		AppPathPtr[ChrPos] = 0x00;  // "C:\VGMPlay\"
//		AppPaths[CurPath] = AppPathPtr;
//		CurPath ++;
//		AppPathPtr += ChrPos + 1;
//	}
	VGMPlay_Init2();

	if (!OpenVGMFile((const char *)&fileinfo))
	{
		fprintf(stderr, "vgm2pcm: error: failed to open vgm_file\n");
		return 1;
	}

	maxBufferSamples = SampleRate / 1;
	sampleBuffer = (WAVE_16BS*)malloc(SAMPLESIZE * maxBufferSamples);
	if (sampleBuffer == NULL)
	{
		fprintf(stderr, "vgm2pcm: error: failed to allocate %u bytes of memory\n", SAMPLESIZE * SampleRate);
		return 1;
	}

	PlayVGM();
	bufferedSamples = 0;
	return 0;
}

int vgm_play_data(void *outBuffer, int size)
{
	static UINT32 currentSample = 0;
	UINT32 index = 0;
	UINT8 result;
	size = size / SAMPLESIZE * 2;
//	char *AppName;
//	char* AppPathPtr;
//	const char *StrPtr;
//	UINT8 CurPath;
//	UINT32 ChrPos;
	if (EndPlay)
	{
		return 0;
	}

	while (1)
	{
	if ( !bufferedSamples )
	{
		bufferedSamples = FillBuffer(sampleBuffer, maxBufferSamples);
		currentSample = 0;
	}

	if ( bufferedSamples )
	{
		UINT32 numberOfSamples;
		const UINT16* sampleData = (UINT16*)sampleBuffer;
		UINT16* outSampleData = (UINT16*)outBuffer;
		sampleData = (UINT16*)sampleBuffer;

		while ((currentSample != SAMPLESIZE * bufferedSamples / 2) && size )
		{
			outSampleData[index] = sampleData[currentSample];
			currentSample++;
			index++;
			size--;
		}
		if ( currentSample == SAMPLESIZE * bufferedSamples / 2 )
		{
			bufferedSamples = 0;
		}
	}
	if (!size)
	{
		break;
	}

	}
	return index * SAMPLESIZE / 2;
}

int vgm_play_stop(void)
{
	StopVGM();

	CloseVGMFile();

	VGMPlay_Deinit();

	return 0;
}

#endif
