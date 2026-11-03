#ifndef CCTOOLS_COMPAT_MACH_MACH_H
#define CCTOOLS_COMPAT_MACH_MACH_H

#include <stdint.h>
#include <mach/boolean.h>
#include <mach/machine.h>
#include <mach/message.h>
#include <mach/vm_prot.h>
#include <mach/machine/vm_types.h>

typedef int kern_return_t;
typedef unsigned int mach_port_t;
typedef mach_port_t host_t;
typedef mach_port_t vm_map_t;
typedef uintptr_t vm_address_t;
typedef int *host_info_t;
typedef int host_flavor_t;
typedef int vm_sync_t;

#define KERN_SUCCESS 0
#define KERN_FAILURE 5
#define MACH_PORT_NULL 0

#define HOST_BASIC_INFO 1
#define HOST_SCHED_INFO 3

struct host_basic_info {
    integer_t max_cpus;
    integer_t avail_cpus;
    natural_t memory_size;
    cpu_type_t cpu_type;
    cpu_subtype_t cpu_subtype;
};
typedef struct host_basic_info host_basic_info_data_t;
#define HOST_BASIC_INFO_COUNT \
    ((mach_msg_type_number_t)(sizeof(host_basic_info_data_t) / sizeof(integer_t)))

struct host_sched_info {
    integer_t min_timeout;
    integer_t min_quantum;
};
typedef struct host_sched_info host_sched_info_data_t;
#define HOST_SCHED_INFO_COUNT \
    ((mach_msg_type_number_t)(sizeof(host_sched_info_data_t) / sizeof(integer_t)))

typedef int vm_machine_attribute_t;
typedef int vm_machine_attribute_val_t;
#define MATTR_CACHE 1
#define MATTR_VAL_ICACHE_FLUSH 9

kern_return_t vm_machine_attribute(vm_map_t task, vm_address_t address, vm_size_t size,
                                   vm_machine_attribute_t attribute,
                                   vm_machine_attribute_val_t *value);

#define VM_SYNC_ASYNCHRONOUS 0x1
#define VM_SYNC_SYNCHRONOUS 0x2
#define VM_SYNC_INVALIDATE 0x4

extern vm_size_t vm_page_size;

mach_port_t mach_task_self(void);
mach_port_t mach_host_self(void);
kern_return_t mach_port_deallocate(mach_port_t task, mach_port_t name);
kern_return_t host_info(host_t host, host_flavor_t flavor, host_info_t info,
                        mach_msg_type_number_t *count);
kern_return_t vm_allocate(vm_map_t task, vm_address_t *address, vm_size_t size, int anywhere);
kern_return_t vm_deallocate(vm_map_t task, vm_address_t address, vm_size_t size);
kern_return_t vm_msync(vm_map_t task, vm_address_t address, vm_size_t size, vm_sync_t flags);
kern_return_t map_fd(int fd, vm_offset_t offset, vm_offset_t *va, boolean_t findspace,
                     vm_size_t size);

#endif
