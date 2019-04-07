SHELL := /bin/bash

CC = i686-elf-gcc
LD = i686-elf-gcc -T
AS = i686-elf-as

CFLAGS = -std=gnu99 -ffreestanding -fno-asynchronous-unwind-tables -g -O3 -Wall -Wextra
LDFLAGS = -ffreestanding -O3 -nostdlib -lgcc

ASOBJ = boot.o
COBJ = main.o vga.o string.o segment.o debug.o interrupt.o irq.o keyboard.o keystate.o qwerty.o istream.o pmap.o

ASOBJP = $(addprefix bin/, $(ASOBJ))
COBJP = $(addprefix bin/, $(COBJ))

default: bin/os.bin

bin/os.bin: $(ASOBJP) $(COBJP)
	$(LD) src/linker.ld -o bin/os.bin $(LDFLAGS) $(ASOBJP) $(COBJP)

$(ASOBJP): bin/%.o: src/%.s
	$(AS) $< -o $@

$(COBJP): bin/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@
	
clean:
	rm -rf bin/*.bin bin/*.o bin/*.iso bin/isodir
	
bin/os.iso: bin/os.bin
	mkdir -p bin/isodir/boot/grub
	cp bin/os.bin bin/isodir/boot/os.bin
	cp src/grub.cfg bin/isodir/boot/grub/grub.cfg
	grub-mkrescue -o bin/os.iso bin/isodir
	rm -rf bin/isodir

debug: bin/os.bin
	qemu-system-i386 -S -s -kernel bin/os.bin &
	gdb -tui -x debug.gdb
