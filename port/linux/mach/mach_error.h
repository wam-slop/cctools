#ifndef CCTOOLS_COMPAT_MACH_MACH_ERROR_H
#define CCTOOLS_COMPAT_MACH_MACH_ERROR_H

#include <mach/mach.h>

const char *mach_error_string(kern_return_t error);

#endif
