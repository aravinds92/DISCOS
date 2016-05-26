	.globl stack
	.bss
	.align 0x1000
	.text	
	.globl _start 	
_start:
        jmp first

        /* Multiboot header -- Safe to place this header in 1st page of memory for GRUB */
        .align 4
        .long 0x1BADB002 /* Multiboot magic number */
        .long 0x00000003 /* Align modules to 4KB, req. mem size */
                         /* See 'info multiboot' for further info */
        .long 0xE4524FFB /* Checksum */
 
gdt_ptr: 
	.short 0x7FF
	.long gdt

gdt:
	.long 0
	.long 0
	.long 0xFFFF
	.long 0x00CF9A00
	.long 0x0000FFFF
	.long 0x00CF9200

		         
first:
	call gdt_init

code:
	movl $0x90000, %esp
	pushl %ebx
	call mem
	popl %ebx
	hlt
	
gdt_init:
    lgdt gdt_ptr       
    mov $0x10, %ax
    movw %ax, %es
	movw %ax, %ds
	movw %ax, %fs
    movw %ax, %ss
    movw %ax, %gs        
    ljmp $0x08, $code  
