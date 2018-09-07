#ifndef FLAPJACKOS_KERNEL_INCLUDE_PANIC_H
#define FLAPJACKOS_KERNEL_INCLUDE_PANIC_H

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((noreturn))
void panic(const char *fmt, ...);

__attribute__((noreturn))
void panic2(const char *message,
            unsigned edi,
            unsigned esi,
            unsigned ebp,
            unsigned esp,
            unsigned ebx,
            unsigned edx,
            unsigned ecx,
            unsigned eax,
            bool error_code_present,
            unsigned error_code,
            unsigned eip);

#ifdef __cplusplus
}
#endif

#endif // FLAPJACKOS_KERNEL_INCLUDE_PANIC_H
