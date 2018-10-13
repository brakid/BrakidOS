# BrakidOS
Operating system experiment

## Requirements:
* nasm
* cross-compiler: https://github.com/nativeos/homebrew-i386-elf-toolchain

## Currently implemented:
* printing to screen
* interupt handling
  * timer 
  * keyboard
* dynamic memory management (first fit strategy)
* reading user input

## TODOs:
* preemptive taskscheduling
** including task manager to start new processes
* file system

The kernel will be built into a floppy disk like format. It comes with its own rudimentary bootloader that initializes the GDT, enables the A20 gate and loads the rest of the kernel from floppy disk.
