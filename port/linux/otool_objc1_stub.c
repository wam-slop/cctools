#include <stdio.h>
#include "mach-o/loader.h"
#include "stuff/bytesex.h"
#include "stuff/symbol.h"
#include "ofile_print.h"

static void objc1_unsupported(void)
{
    printf("Objective-C 1 metadata printing is not supported by this otool build\n");
}

void print_objc_segment(struct load_command *load_commands, uint32_t ncmds, uint32_t sizeofcmds,
                        enum byte_sex object_byte_sex, char *object_addr,
                        unsigned long object_size, struct symbol *sorted_symbols,
                        unsigned long nsorted_symbols, enum bool verbose)
{
    objc1_unsupported();
}

void print_objc_protocol_section(struct load_command *load_commands, uint32_t ncmds,
                                 uint32_t sizeofcmds, enum byte_sex object_byte_sex,
                                 char *object_addr, unsigned long object_size,
                                 enum bool verbose)
{
    objc1_unsupported();
}

void print_objc_string_object_section(char *sectname, struct load_command *load_commands,
                                      uint32_t ncmds, uint32_t sizeofcmds,
                                      enum byte_sex object_byte_sex, char *object_addr,
                                      unsigned long object_size, enum bool verbose)
{
    objc1_unsupported();
}

void print_objc_runtime_setup_section(struct load_command *load_commands, uint32_t ncmds,
                                      uint32_t sizeofcmds, enum byte_sex object_byte_sex,
                                      char *object_addr, unsigned long object_size,
                                      enum bool verbose)
{
    objc1_unsupported();
}
