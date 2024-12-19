// @Hustler's Project


#include <asm/entry.h>

OUTPUT_FORMAT("elf64-littleaarch64")
OUTPUT_ARCH("aarch64")
ENTRY(_head)

SECTIONS {
    . = TINY_START;

    __tiny_start = .;

    .head : {
        __head_start = .;
        *(.head .head.*)
        . = ALIGN(8);
        __head_end = .;
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
        . = ALIGN(4096);
        __data_end = .;
    }

    .bss : {
        __bss_start = .;
        *(.bss .bss.*)
        . = ALIGN(8);
        __bss_end = .;
    }

    . = ALIGN(4096);

    __tiny_end = .;
}
