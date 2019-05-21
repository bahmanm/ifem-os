# IFEM OS

Aspiring to be *an operating system designed from the ground up for embedded/industrial
applications with an emphasis on fault-tolerance.* :-)

# Introduction

IFEM is an **experimental, work-in-progress, immature and early-stage**
operating system - you've been warned :-)

The idea behind IFEM is having an operating system where

  - kernel is a minimal and thin piece of functionality
  - modules and drivers don't know anything about each other beyond APIs and
    SPIs
  - and there are no dependecies during compile and link time - instead pieces
    fit together only at runtime.

# Current Status

Version 0.0.1 is the first release where "something works":

  - bootloader loads the OS from the disk (image) into memory and hands the
    control over to kernel.
  - kernel
    * does basic harware init routine
    * enables the protected mode
    * sets up two interrupt handlers (timer and keyboard)
  - interrupt handlers print the keycode on each keyboard interrupt and
    announce a message every 200 timer ticks.

Here's a recording of its execution:

<a href="https://imgur.com/HcDL4vP"><img src="https://i.imgur.com/HcDL4vP.gif" title="source: imgur.com" /></a>

# How To Build

### Using Gnu toolchain on a Linux box

Provided you have installed `gcc` (5+)  and `gcc-multilib` (if on a x64 machine):

```
make clean all
```

### No Gnu toolchain

Provided you have docker already installed:

```
$ docker run --rm -v $(pwd):/src -w /src -it gcc:9
# make clean all
```

# How To Run

IFEM's "runnable" binary is `ifem-os.img` which you can find in the root of the
source tree after a successful build.

### [Bochs](http://bochs.sourceforge.net/)

There's a `bochsrc` file included in the source tree - provided you fix the paths
in that file to match your own Bochs installation, simply run bochs while inside
the source tree.

### Virtualbox

If you intend to run IFEM inside a vbox machine, you need to run `make pad512`
during build so that vbox recognises `ifem-os.img` as a floppy disk image.

### Physical machine

If you still have access to a machine with floppy drive, just write IFEM's
binary to the floppy (e.g. `dd`) and boot your machine!
