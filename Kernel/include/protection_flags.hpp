#ifndef FLAPJACKOS_KERNEL_INCLUDE_PROTECTION_FLAGS_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PROTECTION_FLAGS_HPP

using ProtectionFlags = unsigned;
constexpr unsigned PRESENT = (1 << 0);
constexpr unsigned WRITABLE = (1 << 1);
constexpr unsigned GLOBAL = (1 << 2);
constexpr unsigned SUPERVISOR = (1 << 3);

#endif // FLAPJACKOS_KERNEL_INCLUDE_PROTECTION_FLAGS_HPP