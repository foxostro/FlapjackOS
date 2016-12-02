# Dev Log

## 2016-12-01

Today, I started work on Flapjack OS. I created the Git repo, created some (presently aspirational) build instructions and created some basic build scripts. Next, I'll follow the [Bare Bones Tutorial](http://wiki.osdev.org/Bare_Bones) to create an extremely basic, but bootable, kernel to serve as my starting point.

## 2016-12-02

Today, I followed the [Bare Bones Tutorial](http://wiki.osdev.org/Bare_Bones) to create an extremely basic kernel starting point. The one I wrote merely clears the screen to red and then halts the machine. It boots and runs some code so I can use this as a skeleton around which to put together the build system.

Additionally, I updated the README with instructions for installing the GRUB2 command-line tools from source and started using them to generate a bootable CD-ROM image for the operating system.