#include "odi/odi.h"
#include "odi/core/debug.h"

#include "bootservices/bootservices.h"

#include "arch/simd.h"
#include "arch/gdt.h"
#include "io/interrupts.h"
#include "memory/memory.h"
#include "memory/paging.h"
#include "memory/heap.h"
#include "util/printf.h"

#define STR_MAX_SIZE 65536

void _start(void) {
    init_simd();
    init_memory();
    init_paging();
    init_heap();
    init_gdt();
    init_interrupts();

    odi_hello();
    
    odi_autoconf((void*)get_rsdp_address());

    odi_list_devices();

    u8 buffer[512] = {0};
    u64 size = odi_read("hd0", 0, 1, buffer);
    printf("Read %d sectors from hd0\n", size);

    for (int i = 0; i < 512; i++) {
        printf("%2x ", buffer[i]);
        if (i % 16 == 15) {
            printf("\n");
        }
    }
    printf("\n");

    odi_goodbye();

    while(1);
}