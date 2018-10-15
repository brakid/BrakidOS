# BrakidOS
Operating system experiment

## Requirements:
* nasm
* cross-compiler: https://github.com/nativeos/homebrew-i386-elf-toolchain
* qemu emulator: https://www.qemu.org

## Currently implemented:
* printing to screen
* interupt handling
  * timer 
  * keyboard
* dynamic memory management (first fit strategy)
* reading user input
* preemptive taskscheduling

## TODOs:
* task scheduling strategy
* task manager to start new processes
* garbage collector to free memory after processes have finished
* file system

The kernel will be built into a floppy disk like format. It comes with its own rudimentary bootloader that initializes the GDT, enables the A20 gate and loads the rest of the kernel from floppy disk.
