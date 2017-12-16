libCommon is a library of code shared in common between userspace programs and the kernel. This is explicitly intended to be for basic functionality shared in common between the kernel and user programs.

This is a library of functions on which other libraries may depend, and explicitly has no dependencies of its own. For example, we can put snprintf() and strnlen() into libCommon. However, we cannot put printf() into libCommon beause it depends on the system call interface. We cannot put malloc() into CommonLib because it depends on libMalloc.

libCommon also can be built both for the host system (e.g. for tests) as well as for the target i386 architecture.

To prevent name collisions between headers here and in the host standard library, we keep possibly conflicting headers to the flapjack_libc directory. These are not included in the header search path when building tests on the host. Additionally, within Common, you must refer to possibly conflicting symbols through macros with the uppercase name of the function, e.g. STRNLEN, which are resolved to non-conflicting names when building for the host.