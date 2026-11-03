#ifndef CCTOOLS_COMPAT_SERVERS_BOOTSTRAP_H
#define CCTOOLS_COMPAT_SERVERS_BOOTSTRAP_H

#include <mach/mach.h>

static const mach_port_t bootstrap_port = MACH_PORT_NULL;

static inline kern_return_t bootstrap_look_up(mach_port_t bootstrap, const char *name,
                                              unsigned int *port)
{
    return KERN_FAILURE;
}

#endif
