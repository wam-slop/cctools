#ifndef LD64_COMPAT_MACH_VM_PROT_H
#define LD64_COMPAT_MACH_VM_PROT_H

typedef int vm_prot_t;

#define VM_PROT_NONE ((vm_prot_t)0x00)
#define VM_PROT_READ ((vm_prot_t)0x01)
#define VM_PROT_WRITE ((vm_prot_t)0x02)
#define VM_PROT_EXECUTE ((vm_prot_t)0x04)
#define VM_PROT_DEFAULT (VM_PROT_READ | VM_PROT_WRITE)
#define VM_PROT_ALL (VM_PROT_READ | VM_PROT_WRITE | VM_PROT_EXECUTE)

#endif
