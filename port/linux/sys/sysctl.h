#ifndef CCTOOLS_COMPAT_SYS_SYSCTL_H
#define CCTOOLS_COMPAT_SYS_SYSCTL_H

#include <stddef.h>

#define CTL_KERN 1
#define KERN_OSRELEASE 2
#define KERN_OSVERSION 65

int sysctl(int *name, unsigned int namelen, void *oldp, size_t *oldlenp, void *newp,
           size_t newlen);

#endif
