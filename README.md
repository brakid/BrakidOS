# BrakidOS
Operating system experiment

## Requirements:
* nasm
* cross-compiler: https://github.com/nativeos/homebrew-i386-elf-toolchain
* qemu emulator: https://www.qemu.org

## Currently implemented:
* printing to screen
* dynamic memory management (first fit strategy)
* interupt handling
  * timer (used for preemtive taskscheduling and getting system time)
  * keyboard (supports US keyboard + shift)
* reading user input (blocking timer interupts)
* preemptive taskscheduling (round robin strategy)
  * yielding remaining processign time
  * idle process (picked if no other task is ready)
* garbage collector to free memory after processes have finished
* utility methods: number -> string, string -> number

## TODOs:
* task manager to start new processes (select a program -> create a running process)
* file system

The kernel will be built into a floppy disk like format. It comes with its own rudimentary bootloader that initializes the GDT, enables the A20 gate and loads the rest of the kernel from floppy disk.
