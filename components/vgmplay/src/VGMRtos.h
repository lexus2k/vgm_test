#ifndef _VGM_RTOS_H_
#define _VGM_RTOS_H_

#include <stdint.h>

#ifdef VGM_NO_ZLIB

#define Z_OK            0
#define Z_STREAM_END    1

#ifndef voidp
#define voidp void*
#endif

#ifndef SEEK_SET
#define SEEK_SET 0
#endif

typedef struct
{
    const uint8_t * data;
    int position;
    int size;
} gzFile_s;

typedef gzFile_s *gzFile;

extern gzFile gzopen(const char *path, const char *mode);
extern int gzread(gzFile file, voidp buf, unsigned len);
extern int gzclose(gzFile file);
extern int gzrewind(gzFile file);
extern int gzseek(gzFile file, int offset, int whence);
extern int gztell(gzFile file);

#endif

#ifdef VGM_RTOS

typedef struct
{
    const uint8_t *data;
    int size;
} vgm_progmem_t;

#endif

#endif

