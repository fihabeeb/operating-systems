all: loader header kernel.elf isoimg run
#header loader kernel isoimg run


#Make trhe first 3 in source
#Put ioa in source???
OBJECTS = source/loader.o \
source/kmain.o \
source/ioa.o \
drivers/driver.o \
drivers/hardware_interrupt_enabler.o \
drivers/interrupt_asm.o \
drivers/interrupt_handlers.o \
drivers/interrupts.o \
drivers/keyboard.o \
drivers/pic.o

CFLAGS = -I. -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
-nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
LDFLAGS = -T ./source/link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf


kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf
#mv ./kernel.elf ./iso/boot

isoimg:  kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	genisoimage -R \
		-b boot/grub/stage2_eltorito \
		-no-emul-boot \
		-boot-load-size 4 \
		-A os \
		-input-charset utf8 \
		-quiet \
		-boot-info-table \
		-o os.iso \
		iso

run: os.iso
	qemu-system-i386 -curses \
		-monitor telnet::47601,server,nowait \
		-chardev stdio,id=char0 \
		-serial chardev:char0 \
		-boot d \
		-cdrom os.iso \
		-m 32 \
		-d cpu \
		-no-reboot \
		-no-shutdown \
		-D logQ.txt



%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o source/*.o drivers/*.o kernel.elf os.iso ios/boot/kernel.elf


loader: loader.asm
	gcc -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c kmain.c -o source/kmain.o
	nasm -f elf loader.asm -o source/loader.o

header: drivers/io.asm
	gcc -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c drivers/io.h -o drivers/ioc.o
	gcc -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c drivers/driver.c -o drivers/driver.o
	nasm -f elf drivers/io.asm -o source/ioa.o

nothing: loader.asm
