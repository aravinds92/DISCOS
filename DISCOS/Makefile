CC = gcc
CFLAGS = -m32 -fno-builtin -fno-stack-protector -fno-strict-aliasing -fno-delete-null-pointer-checks -nostdinc -I. -g -std=c99
#CPPFLAGS = -Wa, --32 -MMD
OBJS = boot.o fifos.o functions.o play_fs.o fs_lib1.o fs_lib2.o fs_lib3.o
PROGS = FIFOS
MNT_POINT=/mnt/C/boot/
INSTALL = install
ASFLAGS = --32

all: $(PROGS)

FIFOS: $(OBJS) 
	$(LD) -m elf_i386 -T memos.ld -o $@ $^
	qemu-system-i386 -kernel FIFOS

%: %.c 

clean:
	-rm -f *.o *.d $(PROGS)
	rm -f /mnt/C/boot/$(PROGS)
	
		
-include *.d
