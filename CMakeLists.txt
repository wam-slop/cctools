cmake_minimum_required(VERSION 3.20)
project(cctools VERSION 698.1 LANGUAGES C)

set(port "${CMAKE_CURRENT_SOURCE_DIR}/port")
set(CCTOOLS_PROGRAM_PREFIX "" CACHE STRING "prefix for installed tool names, e.g. powerpc-apple-darwin9-")

add_library(cctools_options INTERFACE)
target_include_directories(cctools_options INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_compile_options(cctools_options INTERFACE -w)
if(NOT APPLE)
  target_include_directories(cctools_options BEFORE INTERFACE "${port}/linux")
  target_compile_options(cctools_options INTERFACE
    -include "${port}/linux/cctools-compat.h" -fcommon -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0)
  target_compile_definitions(cctools_options INTERFACE
    __LITTLE_ENDIAN__=1 _GNU_SOURCE=1 _DARWIN_C_SOURCE=1)
endif()

set(libstuff_sources
  allocate.c errors.c arch.c execute.c bytesex.c round.c hppa.c
  swap_headers.c reloc.c version_number.c get_toc_byte_sex.c
  best_arch.c fatals.c arch_usage.c ofile.c ofile_error.c print.c
  set_arch_flag_name.c seg_addr_table.c dylib_table.c
  breakout.c writeout.c checkout.c fatal_arch.c ofile_get_word.c
  vm_flush_cache.c hash_string.c dylib_roots.c guess_short_name.c
  SymLoc.c get_arch_from_host.c crc32.c macosx_deployment_target.c
  symbol_list.c unix_standard_mode.c lto.c)
list(TRANSFORM libstuff_sources PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/libstuff/")

add_library(cctools_platform STATIC "${port}/common/vers.c")
target_link_libraries(cctools_platform PUBLIC cctools_options)
if(NOT APPLE)
  target_sources(cctools_platform PRIVATE "${port}/linux/cctools-compat.c")
endif()

add_library(stuff STATIC ${libstuff_sources})
target_link_libraries(stuff PUBLIC cctools_platform)

function(cctools_tool name)
  cmake_parse_arguments(tool "NO_LIBSTUFF" "" "SOURCES;DEFINES" ${ARGN})
  add_executable(${name} ${tool_SOURCES})
  set_target_properties(${name} PROPERTIES OUTPUT_NAME "${CCTOOLS_PROGRAM_PREFIX}${name}")
  target_compile_definitions(${name} PRIVATE ${tool_DEFINES})
  if(tool_NO_LIBSTUFF)
    target_link_libraries(${name} PRIVATE cctools_platform)
  else()
    target_link_libraries(${name} PRIVATE stuff)
  endif()
  install(TARGETS ${name} RUNTIME DESTINATION bin)
endfunction()

set(ar_sources append.c ar.c archive.c contents.c delete.c extract.c misc.c move.c
               print.c replace.c)
list(TRANSFORM ar_sources PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/ar/")
cctools_tool(ar NO_LIBSTUFF SOURCES ${ar_sources}
             "${CMAKE_CURRENT_SOURCE_DIR}/libstuff/execute.c"
             "${CMAKE_CURRENT_SOURCE_DIR}/libstuff/allocate.c"
             "${CMAKE_CURRENT_SOURCE_DIR}/libstuff/fatals.c"
             "${CMAKE_CURRENT_SOURCE_DIR}/libstuff/unix_standard_mode.c")

set(misc "${CMAKE_CURRENT_SOURCE_DIR}/misc")
cctools_tool(libtool SOURCES ${misc}/libtool.c)
cctools_tool(nm SOURCES ${misc}/nm.c)
cctools_tool(strip SOURCES ${misc}/strip.c)
cctools_tool(nmedit SOURCES ${misc}/strip.c DEFINES NMEDIT)
cctools_tool(lipo SOURCES ${misc}/lipo.c)
cctools_tool(install_name_tool SOURCES ${misc}/install_name_tool.c)
cctools_tool(size SOURCES ${misc}/size.c)
cctools_tool(strings SOURCES ${misc}/strings.c)

set(otool_sources main.c ofile_print.c m68k_disasm.c i860_disasm.c m88k_disasm.c
                  i386_disasm.c ppc_disasm.c hppa_disasm.c sparc_disasm.c
                  arm_disasm.c print_objc2_64bit.c print_objc2_32bit.c)
if(APPLE)
  list(APPEND otool_sources print_objc.c)
endif()
list(TRANSFORM otool_sources PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/otool/")
if(NOT APPLE)
  list(APPEND otool_sources "${port}/linux/otool_objc1_stub.c")
endif()
add_library(otool_ofile OBJECT "${CMAKE_CURRENT_SOURCE_DIR}/libstuff/ofile.c")
target_compile_definitions(otool_ofile PRIVATE OTOOL)
target_include_directories(otool_ofile PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/otool")
target_link_libraries(otool_ofile PUBLIC cctools_options)
cctools_tool(otool SOURCES ${otool_sources} $<TARGET_OBJECTS:otool_ofile>)
target_include_directories(otool PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/otool")

add_custom_command(TARGET libtool POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink "${CCTOOLS_PROGRAM_PREFIX}libtool"
          "$<TARGET_FILE_DIR:libtool>/${CCTOOLS_PROGRAM_PREFIX}ranlib")
install(CODE "file(CREATE_LINK ${CCTOOLS_PROGRAM_PREFIX}libtool \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/bin/${CCTOOLS_PROGRAM_PREFIX}ranlib\" SYMBOLIC)")
