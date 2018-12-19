#ifndef FLAPJACKOS_KERNEL_INCLUDE_PANIC_H
#define FLAPJACKOS_KERNEL_INCLUDE_PANIC_H

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((noreturn))
void panic(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif // FLAPJACKOS_KERNEL_INCLUDE_PANIC_H
