#include <sys/types.h>
#include <sys/mman.h>
#include <mach/mach.h>

kern_return_t map_fd(int fd, vm_offset_t offset, vm_offset_t *va, boolean_t findspace,
                     vm_size_t size)
{
    void *mapping;
    if (size == 0) {
        *va = 0;
        return KERN_SUCCESS;
    }
    mapping = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, (off_t)offset);
    if (mapping == MAP_FAILED)
        return KERN_FAILURE;
    *va = (vm_offset_t)mapping;
    return KERN_SUCCESS;
}
