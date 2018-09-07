#ifndef FLAPJACKOS_KERNEL_INCLUDE_MISC_H
#define FLAPJACKOS_KERNEL_INCLUDE_MISC_H

#define DWORD_UPPER_WORD(value) (unsigned short)( ((value) >> 16) & 0xFFFF )
#define DWORD_LOWER_WORD(value) (unsigned short)(  (value)        & 0xFFFF )

#define WORD_UPPER_BYTE(value) (unsigned char)( ((value) >> 8)    & 0xFF )
#define WORD_LOWER_BYTE(value) (unsigned char)(  (value)          & 0xFF )

#endif // FLAPJACKOS_KERNEL_INCLUDE_MISC_H
