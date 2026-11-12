#ifndef CCTOOLS_COMPAT_MAKE_H
#define CCTOOLS_COMPAT_MAKE_H

#include <mach/mach.h>

static inline kern_return_t make_alert(mach_port_t port, int type, const char *function,
                                       unsigned int function_length, const char *file,
                                       unsigned int file_length, const char *a,
                                       unsigned int a_length, int line, const char *message,
                                       unsigned int message_length)
{
    return KERN_FAILURE;
}

#endif
