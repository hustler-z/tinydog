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

    .data : {
        __data_start = .;
        *(.data .data.*)
        . = ALIGN(8);
        __data_end = .;
    }

    .rodata : {
        __rodata_start = .;
        /* console commands */
        __cmd_list__ = .;
        KEEP(*(.cmd_list))
        __cmd_auto__ = .;
        KEEP(*(.cmd_auto))
        KEEP(*(.cmd_end))
        *(.rodata .rodata.*)
        . = ALIGN(8);
        __rodata_end = .;
    }

    .bss : {
        __bss_start = .;
        *(.bss .bss.*)
        . = ALIGN(8);
        __bss_end = .;
    }

    . = ALIGN(4096);

    // tiny stack
    .stack : {
        __stack_start = .;
        *(.stack .stack.*)
        . = ALIGN(4096);
        __stack_end = .;
    }

    __tiny_end = .;
}
