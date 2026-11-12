#include "cctools-compat.h"

#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <mach/mach.h>
#include <mach/mach_error.h>
#include <sys/sysctl.h>

vm_size_t vm_page_size = 4096;

__attribute__((constructor)) static void init_page_size(void)
{
    vm_page_size = (vm_size_t)sysconf(_SC_PAGESIZE);
}

mach_port_t mach_task_self(void) { return 1; }
mach_port_t mach_host_self(void) { return 2; }

kern_return_t mach_port_deallocate(mach_port_t task, mach_port_t name)
{
    return KERN_SUCCESS;
}

kern_return_t host_info(host_t host, host_flavor_t flavor, host_info_t info,
                        mach_msg_type_number_t *count)
{
    if (flavor == HOST_BASIC_INFO) {
        host_basic_info_data_t *basic = (host_basic_info_data_t *)info;
        memset(basic, 0, sizeof(*basic));
        basic->max_cpus = basic->avail_cpus = (integer_t)sysconf(_SC_NPROCESSORS_ONLN);
        basic->cpu_type = CPU_TYPE_X86_64;
        basic->cpu_subtype = CPU_SUBTYPE_X86_64_ALL;
        *count = HOST_BASIC_INFO_COUNT;
        return KERN_SUCCESS;
    }
    if (flavor == HOST_SCHED_INFO) {
        host_sched_info_data_t *sched = (host_sched_info_data_t *)info;
        sched->min_timeout = 10;
        sched->min_quantum = 10;
        *count = HOST_SCHED_INFO_COUNT;
        return KERN_SUCCESS;
    }
    return KERN_FAILURE;
}

kern_return_t vm_allocate(vm_map_t task, vm_address_t *address, vm_size_t size, int anywhere)
{
    void *mapping = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mapping == MAP_FAILED)
        return KERN_FAILURE;
    *address = (vm_address_t)mapping;
    return KERN_SUCCESS;
}

kern_return_t vm_deallocate(vm_map_t task, vm_address_t address, vm_size_t size)
{
    if (size == 0)
        return KERN_SUCCESS;
    return munmap((void *)address, size) == 0 ? KERN_SUCCESS : KERN_FAILURE;
}

kern_return_t vm_msync(vm_map_t task, vm_address_t address, vm_size_t size, vm_sync_t flags)
{
    return msync((void *)address, size, MS_SYNC) == 0 ? KERN_SUCCESS : KERN_FAILURE;
}

kern_return_t vm_machine_attribute(vm_map_t task, vm_address_t address, vm_size_t size,
                                   vm_machine_attribute_t attribute,
                                   vm_machine_attribute_val_t *value)
{
    return KERN_SUCCESS;
}

const char *mach_error_string(kern_return_t error)
{
    return error == KERN_SUCCESS ? "success" : "failure";
}

int sysctl(int *name, unsigned int namelen, void *oldp, size_t *oldlenp, void *newp,
           size_t newlen)
{
    const char *value;
    size_t length;
    if (namelen != 2 || name[0] != CTL_KERN) {
        errno = ENOENT;
        return -1;
    }
    if (name[1] == KERN_OSRELEASE)
        value = "9.8.0";
    else if (name[1] == KERN_OSVERSION)
        value = "9L31a";
    else {
        errno = ENOENT;
        return -1;
    }
    length = strlen(value) + 1;
    if (oldp != NULL) {
        if (*oldlenp < length) {
            errno = ENOMEM;
            return -1;
        }
        memcpy(oldp, value, length);
    }
    *oldlenp = length;
    return 0;
}

int _NSGetExecutablePath(char *buf, uint32_t *bufsize)
{
    ssize_t length = readlink("/proc/self/exe", buf, *bufsize - 1);
    if (length < 0)
        return -1;
    buf[length] = '\0';
    return 0;
}

void strmode(mode_t mode, char *bp)
{
    switch (mode & S_IFMT) {
    case S_IFDIR: *bp++ = 'd'; break;
    case S_IFCHR: *bp++ = 'c'; break;
    case S_IFBLK: *bp++ = 'b'; break;
    case S_IFLNK: *bp++ = 'l'; break;
    case S_IFSOCK: *bp++ = 's'; break;
    case S_IFIFO: *bp++ = 'p'; break;
    default: *bp++ = '-'; break;
    }
    *bp++ = (mode & S_IRUSR) ? 'r' : '-';
    *bp++ = (mode & S_IWUSR) ? 'w' : '-';
    if (mode & S_ISUID)
        *bp++ = (mode & S_IXUSR) ? 's' : 'S';
    else
        *bp++ = (mode & S_IXUSR) ? 'x' : '-';
    *bp++ = (mode & S_IRGRP) ? 'r' : '-';
    *bp++ = (mode & S_IWGRP) ? 'w' : '-';
    if (mode & S_ISGID)
        *bp++ = (mode & S_IXGRP) ? 's' : 'S';
    else
        *bp++ = (mode & S_IXGRP) ? 'x' : '-';
    *bp++ = (mode & S_IROTH) ? 'r' : '-';
    *bp++ = (mode & S_IWOTH) ? 'w' : '-';
    if (mode & S_ISVTX)
        *bp++ = (mode & S_IXOTH) ? 't' : 'T';
    else
        *bp++ = (mode & S_IXOTH) ? 'x' : '-';
    *bp++ = ' ';
    *bp = '\0';
}
