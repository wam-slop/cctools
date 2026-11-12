#ifndef CCTOOLS_DARWIN_COMPAT_H
#define CCTOOLS_DARWIN_COMPAT_H

#ifdef __LP64__
#include <mach/boolean.h>
#include <mach/kern_return.h>
#include <mach/vm_types.h>

#define SWAP_LONG(a) SWAP_INT(a)

kern_return_t map_fd(int fd, vm_offset_t offset, vm_offset_t *va, boolean_t findspace,
                     vm_size_t size);
#endif

#endif
