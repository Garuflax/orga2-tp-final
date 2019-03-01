# ==================================================================================
# TRABAJO PRACTICO FINAL - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
# ==================================================================================

MCOPY=mcopy

BOOT_SRC=boot.asm
BOOT_BIN=boot.bin

KERNEL_SRC=kernel.asm
KERNEL_OBJ=kernel.o
KERNEL_BIN=kernel.bin

DEPEND = Makefile \
	kernel.asm a20.asm \
	isr.mac isr.asm isr.h \
	pic.c pic.h i386.h idt.c idt.h screen.c screen.h tss.c tss.h mmu.c mmu.h sched.c sched.h game.c game.h

OBJ=$(KERNEL_OBJ) pic.o idt.o isr.o screen.o tss.o mmu.o sched.o game.o
DISK_IMG=diskette.img

CFLAGS=-fno-pie -no-pie -m64 -g -ggdb -Wall -Werror -O0 \
  -fno-zero-initialized-in-bss -fno-stack-protector -ffreestanding -mno-red-zone

TASKS=master.c manager.c player.c enemy.c

TASKS_OBJ=master.o manager.o player.o enemy.o

TASKS_BIN=master.tsk manager.tsk player.tsk enemy.tsk

CODE_SECTORS=32      # 16KB
TASKS_SECTORS=32     # 16KB
KERNELSIZE=64        # 0x40 = CODE_SECTORS + TASKS_SECTORS = 32KB

TASK_CODE_SECTORS=8  # 4KB

TASKSTARTADDR=0x200000

CC=gcc
NASM=nasm
NASMFLAGS=-felf64
OBJCOPY=objcopy
NM=nm
LD=ld
LDTASKFLAGS=-s -static -m elf_x86_64 --oformat binary -b elf64-x86-64 -e task -Ttext $(TASKSTARTADDR)
LDFLAGS=-static -m elf_x86_64 -b elf64-x86-64 -e start -Ttext 0x1000

QUIET = @

.PHONY=clean all

all: clean diskette.img

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

%.o: %.asm
	$(NASM) $(NASMFLAGS) -o $@ $^

%.tsk: %.o
	@echo 'Linkeando tarea...'
	$(LD) $(LDTASKFLAGS) -o $@.tmp $^
	@echo ''
	mv $@.tmp $@

kernel.bin: $(OBJ)
	@echo 'Linkeando kernel...'
	$(LD) $(LDFLAGS) -o $@.tmp $(OBJ)
	$(NM) $@.tmp | grep -v " U " | awk '{print "0x" $$1 " " $$3}' > kernel.sym
	$(OBJCOPY) -S -O binary $@.tmp $@
	rm $@.tmp
	@echo ''

boot.bin: $(BOOT_SRC)
	$(NASM) -fbin $(BOOT_SRC) -o $(BOOT_BIN)

diskette.img: $(BOOT_BIN) $(KERNEL_BIN) $(DEPEND) $(TASKS_BIN)
	mv $(BOOT_BIN) boot.bin.tmp
	mv $(KERNEL_BIN) kernel.bin.tmp
	dd bs=512 count=2880 if=/dev/zero of=$(DISK_IMG)
	mkfs.msdos -F 12 $(DISK_IMG) -n BOOTLOADER
	dd if=boot.bin.tmp of=$(DISK_IMG) count=1 seek=0 conv=notrunc
	dd if=kernel.bin.tmp count=$(CODE_SECTORS) of=$(DISK_IMG) seek=1  conv=notrunc
	rm boot.bin.tmp
	rm kernel.bin.tmp
	dd if=master.tsk count=$(TASK_CODE_SECTORS) of=$(DISK_IMG) seek=33  conv=notrunc
	dd if=manager.tsk count=$(TASK_CODE_SECTORS) of=$(DISK_IMG) seek=41  conv=notrunc
	dd if=player.tsk count=$(TASK_CODE_SECTORS) of=$(DISK_IMG) seek=49  conv=notrunc
	dd if=enemy.tsk count=$(TASK_CODE_SECTORS) of=$(DISK_IMG) seek=57  conv=notrunc

run: $(DISK_IMG)
	bochs -q -rc bochsdbg

clean:
	rm -f *.o
	rm -f *.bin
	rm -f *.tmp
	rm -f *.ctsko
	rm -f kernel.sym
	rm -f *.ctsk
	rm -f *.tsko
	rm -f *.tsk
	rm -f $(DISK_IMG)
	rm -rf *~