# Flapjack OS

Flapjack OS is a basic operating system for i386 computers. What is this operating system going to be? I want something basic, just a toy, which exists for my own amusement and is an end in itself. The joy of this project will come from my developing it and not from the end result.

Flapjack is going to be a 32-bit multitasking, preemptive kernel with virtual memory. It will eventually be a sort of pseudo-unix system. Though, by no means will I attempt to implement the POSIX standard.

I'm developing Flapjack on macOS and all instructions will assume you are too. It is possible to get the thing to build and run on Linux but that is left as an exercise for the reader. You'll need to install a number of dependencies and tools first.

## Building and Installing Dependencies and Tools

1. Install [MacPorts](https://www.macports.org).
2. Make sure your environment exports a variable FLAPJACKOS_BUILD_DIR which defines the path to the build output directory. Insert this into "$HOME/.profile":

   ```bash
   export FLAPJACKOS_BUILD_DIR="$HOME/FlapjackOS-build"
   ```
   
   and then make sure that directory exists:
   ```bash
   mkdir -p "$FLAPJACKOS_BUILD_DIR"
   ```

3. Build a cross compiler:

   ```bash
   % ./scripts/make_toolchain.sh i386-elf
   % rm -rf $FLAPJACKOS_BUILD_DIR/build_toolchain/
   % ./scripts/make_toolchain.sh x86_64-elf
   ```

   * Make sure to put the cross compiler into your $PATH.

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
   % sudo port install objconv automake autoconf
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

The build script will generate CMake build files, build a bootable iso, and build unit tests:

   ```bash
   % ./scripts/build.sh
   ```

To run the unit tests, run the test script, or cd to the build_host directory and run ctest:

   ```bash
   % ./scripts/test.sh
   ```

## Running Flapjack OS

### Running on Bochs

1. Install Bochs. You should not install from MacPorts. Instead build and install from source so that we can can enable debugging features and different display libraries. The Bochs which comes from MacPorts seems to only support the X11 display driver, for example, and will not work correctly unless you also install [XQuartz](https://www.xquartz.org).

   ```bash
   % sudo port install libsdl2 pkgconfig
   % svn co http://svn.code.sf.net/p/bochs/code/tags/REL_2_6_8_FINAL/bochs boch
   % cd bochs
   % CXX=/usr/bin/clang++ CC=/usr/bin/clang ./configure --enable-smp --enable-cpu-level=6 --enable-all-optimizations --enable-x86-64 --enable-pci --enable-vmx --enable-debugger --enable-disasm --enable-debugger-gui --enable-logging --enable-fpu --enable-3dnow --enable-sb16=dummy --enable-cdrom --enable-x86-debugger --enable-iodebug --disable-plugins --disable-docbook --with-term --with-sdl2
   % make
   % sudo make install
   ```

2. An example bochsrc file may be found in bochsrc.template. Copy this to bochsrc and tweak as needed.
3. Build Flapjack OS so that you have the "FlapjackOS.iso" disk image.
4. Launch Bochs with the "run_bochs.sh" helper script:

   ```bash
   % ./scripts/run_bochs.sh i386
   ```

Notes:
* Bochs 2.6.9 does not build with Clang.
* Bochs 2.6.8 and 2.6.9 do not build with GCC8.

### Running on Qemu

1. Install Qemu:

   ```bash
   % sudo port install qemu
   ```

2. Build Flapjack OS so that you have the "FlapjackOS.iso" disk image.
3. Launch Qemu as follows:

   ```bash
   % ./scripts/run_qemu.sh i386
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

* The build script will make use of Ninja and Ccache if those tools are installed and available in the PATH.
* Use the script "mount_bootfd.sh" to mount the boot disk image. This will permit you to see what files it contains.
* Qemu is capable of directly booting a multiboot compliant kernel like so:

   ```bash
   % qemu-system-i386 -kernel ./kernel.bin
   ```
