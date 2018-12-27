#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PLATFORM_NAMER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PLATFORM_NAMER_HPP

namespace i386 {
    class PlatformNamer {
    public:
        const char* get_platform() const
        {
            return "i386";
        }
    };
} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PLATFORM_NAMER_HPP
