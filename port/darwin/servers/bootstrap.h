#ifndef CCTOOLS_DARWIN_SERVERS_BOOTSTRAP_H
#define CCTOOLS_DARWIN_SERVERS_BOOTSTRAP_H

#include <mach/mach.h>

kern_return_t bootstrap_look_up(mach_port_t bp, const char *service_name, mach_port_t *sp);

#endif
