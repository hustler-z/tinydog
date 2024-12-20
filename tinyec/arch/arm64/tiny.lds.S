// @Hustler's Project


#include <asm/entry.h>

OUTPUT_FORMAT("elf64-littleaarch64")
OUTPUT_ARCH("aarch64")
ENTRY(_head)

SECTIONS {
    . = TINY_START;

    __tiny_start = .;

    .text : {
        __text_start = .;
        *(.text.head)
        *(.text.init)
        *(.text .text.*)
        . = ALIGN(8);
        __text_end = .;
    }

    .rodata : {
        __rodata_start = .;
        *(.rodata .rodata.*)
        . = ALIGN(8);
        __rodata_end = .;
    }

    .data : {
        __data_start = .;
        *(.data .data.*)
        . = ALIGN(8);
        __data_end = .;
    }

    .bss : {
        __bss_start = .;
        *(.bss .bss.*)
        . = ALIGN(8);
        __bss_end = .;
    }

    . = ALIGN(4096);

    .heap : {
        __heap_start = .;
        *(.heap .heap.*)
        . = ALIGN(4096);
        __heap_end = .;
    }

    __tiny_end = .;
}
