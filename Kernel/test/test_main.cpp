#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "boot.h"

extern "C" {

char g_kernel_physical_start[1] = {'x'};
char g_boot_end[1] = {'x'};
char g_kernel_image_begin[1] = {'x'};
char g_kernel_text_begin[1] = {'x'};
char g_kernel_text_end[1] = {'x'};
char g_kernel_rodata_begin[1] = {'x'};
char g_kernel_rodata_end[1] = {'x'};
char g_start_ctors[1] = {'x'};
char g_end_ctors[1] = {'x'};
char g_kernel_data_end[1] = {'x'};
char g_kernel_image_end[1] = {'x'};
char g_kernel_physical_end[1] = {'x'};

} // extern "C"
