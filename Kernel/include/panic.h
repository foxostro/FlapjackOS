#pragma once

__attribute__((noreturn))
void panic(const char * restrict fmt, ...);

__attribute__((noreturn))
void panic2(const char * restrict message,
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
