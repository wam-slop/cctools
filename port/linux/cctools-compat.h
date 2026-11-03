#ifndef CCTOOLS_COMPAT_H
#define CCTOOLS_COMPAT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <stdarg.h>
#include <time.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/file.h>

#define __private_extern__ __attribute__((visibility("hidden")))
#define __DARWIN_UNIX03 1

#ifndef EFTYPE
#define EFTYPE 79
#endif
#ifndef EBADRPC
#define EBADRPC 72
#endif
#ifndef AR_EFMT1
#define AR_EFMT1 "#1/"
#endif

#define SWAP_LONG(a) SWAP_INT(a)

#define st_mtimespec st_mtim
#define st_atimespec st_atim

int _NSGetExecutablePath(char *buf, uint32_t *bufsize);
void strmode(mode_t mode, char *bp);

#endif
