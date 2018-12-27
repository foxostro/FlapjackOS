#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_PLATFORM_NAMER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_PLATFORM_NAMER_HPP

namespace x86_64 {
    class PlatformNamer {
    public:
        const char* get_platform() const
        {
            return "x86_64";
        }
    };
} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_PLATFORM_NAMER_HPP
