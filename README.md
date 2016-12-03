# Flapjack OS

Flapjack OS is a basic operating system for i386 computers. What is this operating system going to be? I want something basic, just a toy, which exists for my own amusement and is an end in itself. The joy of this project will come from my developing it and not from the end result.

Flapjack is going to be a 32-bit multitasking, preemptive kernel with virtual memory. It will eventually be a sort of pseudo-unix system. Though, by no means will I attempt to implement the POSIX standard.

I'm developing Flapjack on macOS and all instructions will assume you are too. It is possible to get the thing to build and run on Linux but that is left as an exercise for the reader. You'll need to install a number of dependencies and tools first.

## Building and Installing Dependencies and Tools

1. Install [MacPorts](https://www.macports.org).
2. Use MacPorts to install the following packages:

   ```bash
   % sudo port install i386-elf-gcc objconv check
   ```

   * i386-elf-gcc -- This is the cross-compiler we'll use to build the kernel.
   * objconv -- This tool can convert and manipulate object files in a variety of formats. This is required for the GRUB2 command-line tools.
   * check -- This is a C unit testing framework used by Flapjack OS.

3. Make sure to put the MacPorts cross-compiler and other tools into your $PATH. The cross-compiler in particular will appear there as a suite of tools named i386-elf-*.

4. Install [xorriso](https://www.gnu.org/software/xorriso)

   ```bash
   % wget https://www.gnu.org/software/xorriso/xorriso-1.4.6.tar.gz
   % tar -xzf xorriso-1.4.6.tar.gz
   % cd xorriso-1.4.6
   % ./configure
   % make
   % sudo make install
   ```

5. Install the GRUB2 command-line tools as described on [this wiki page](http://wiki.osdev.org/GRUB):

   ```bash
   % git clone git://git.savannah.gnu.org/grub.git
   % cd grub
   % ./autogen.sh
   % mkdir ../temp
   % cd ../temp
   % ../grub/configure --disable-werror TARGET_CC=i386-elf-gcc TARGET_OBJCOPY=i386-elf-objcopy TARGET_STRIP=i386-elf-strip TARGET_NM=i386-elf-nm TARGET_RANLIB=i386-elf-ranlib --target=i386-elf
   % make
   % sudo make install
   ```

## Building Flapjack OS

1. Use Cmake to generate a makefile and build the bootable iso image:

   ```bash
   % cmake -G 'Unix Makefiles' .
   % make iso
   ```

2. TODO: Run unit tests by building thing the testing target on the command line like so:

   ```bash
   % make test
   ```

## Running Flapjack OS

### Running on Bochs

1. Install Bochs. You should not install from MacPorts. Instead build and install from source so that we can can enable debugging features and different display libraries. The Bochs which comes from MacPorts seems to only support the X11 display driver, for example, and will not work correctly unless you also install [XQuartz](https://www.xquartz.org).

   ```bash
   % svn co http://svn.code.sf.net/p/bochs/code/tags/REL_2_6_8_FINAL/bochs boch
   % cd bochs
   % ./configure --enable-smp --enable-cpu-level=6 --enable-all-optimizations --enable-x86-64 --enable-pci --enable-vmx --enable-debugger --enable-disasm --enable-debugger-gui --enable-logging --enable-fpu --enable-3dnow --enable-sb16=dummy --enable-cdrom --enable-x86-debugger --enable-iodebug --disable-plugins --disable-docbook --with-term --with-sdl2
   % make
   % sudo make install
   ```

2. An example bochsrc file may be found in bochsrc.template. Copy this to bochsrc and tweak as needed.
3. Build Flapjack OS so that you have the "bootfd.img" floppy disk image file.
4. Launch Bochs with the "run.sh" helper script:

   ```bash
   % ./run.sh
   ```

### Running on Qemu

1. Install Qemu:

   ```bash
   % sudo port install qemu
   ```

2. Build Flapjack OS so that you have the "FlapjackOS.iso" disk image.
3. Launch Qemu as follows:

   ```bash
   % qemu-system-i386 -cdrom ./FlapjackOS.iso
   ```

### Running on real hardware

*First a warning:* I don't recommend running the operating system on bare metal unless you are Okay with the machine being accidentally destroyed. For example, if you are Okay with the possibility of some bug overwriting your hard drive then feel free to proceed. Also, I know that some hardware can be damaged by incorrectly written device drivers and take no responsibility whatsoever for what might happen if you try this.

1. Reboot the target machine. From the BIOS, change boot settings so that the machine will attempt to boot from a USB drive first.
2. Find a USB flash drive that you're okay with destroying. At the very least, the drive must be entirely and destructively erased.
2. Insert the USB stick into your Mac.
3. Run the install script and follow the prompts:

   ```bash
   % ./install_to_usb_stick.sh
   ```

4. Eject the flash drive and insert into the target machine.
5. Reboot the target machine.


## Misc. Tips and Tricks

* Use the script "mount_bootfd.sh" to mount the boot disk image. This will permit you to see what files it contains.
* Qemu is capable of directly booting a multiboot compliant kernel like so:

   ```bash
   % qemu-system-i386 -kernel ./kernel.bin
   ```
