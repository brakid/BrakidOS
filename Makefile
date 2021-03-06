SRC_DIR=./src
BUILD_DIR=./build
BOOTLOADER_SRC=$(SRC_DIR)/bootloader/bootloadercpp.asm
BOOTLOADER_TARGET=$(BUILD_DIR)/bootloader.o

KERNEL_SRC=$(SRC_DIR)/*.cpp
BINARY=$(BUILD_DIR)/kernel.bin
DISK=$(BUILD_DIR)/disk.img
DEBUG=-g

clean:
	rm $(BUILD_DIR)/*
all: kernel
bootloader: $(BOOTLOADER_TARGET)
kernel: $(BINARY)

$(BOOTLOADER_TARGET): $(BOOTLOADER_SRC)
	mkdir -p $(BUILD_DIR)
	nasm -f elf32 $(BOOTLOADER_SRC) -o $(BOOTLOADER_TARGET)

$(BINARY): $(KERNEL_SRC) $(BOOTLOADER_TARGET)
	i386-elf-gcc $(KERNEL_SRC) $(BOOTLOADER_TARGET) -o $(BINARY) $(DEBUG) -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -nostdlib -ffreestanding -std=c++11 -mno-red-zone -fno-exceptions -fno-rtti -Wall -Werror -Wextra -T linker.ld -masm=intel

bootdisk: $(BINARY)
	dd if=/dev/zero of=$(DISK) bs=512 count=2880 ; dd conv=notrunc if=$(BINARY) of=$(DISK) bs=512 seek=0
run: $(BINARY)
	qemu-system-i386 -machine q35 -fda $(BINARY) -m 1G
rungdb: $(BINARY)
	qemu-system-i386 -machine q35 -fda $(BINARY) -gdb tcp::26000 -S -m 1G