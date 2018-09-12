Each subdirectory here is named for a target processor supported by FlapjackOS. Within, you'll find architecture-specific start code. Each piece of boot code is intended to be the actual kernel entry point from the bootloader. And, actually, the linker ensures this code is linked as the head of the kernel image.

The purpose of this code is to setup an environment in which it is possible to call kernel_main() and then make that call.
