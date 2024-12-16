OUTPUT_FORMAT("elf32-powerpc")
OUTPUT_ARCH(powerpc:common)

ENTRY(_start)

SECTIONS {
    . = 0x90000020;

    .start : { crt0.o(*) }
    .text : { *(.text) }
    .rodata : { *(.rodata .rodata.*) }
    .data : { *(.data) }

    __bss_start = .;
    .bss : { *(.bss) }
    __bss_end = .;

    . = ALIGN(0x40);
    .stack : {
        . += 0x8000;
        _stack_top = .;
    }
}
