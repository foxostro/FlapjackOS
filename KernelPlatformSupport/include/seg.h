#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_SEG_H
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_SEG_H

#define SEGSEL_KERNEL_TSS_IDX_LO   (1)
#define SEGSEL_KERNEL_TSS_IDX_HI   (2) // unused in 32-bit mode
#define SEGSEL_KERNEL_CS_IDX       (3)
#define SEGSEL_KERNEL_DS_IDX       (4)
#define SEGSEL_USER_CS_IDX         (5)
#define SEGSEL_USER_DS_IDX         (6)

// Declare a segment selector.
// See the Intel manual volume 3a, section 3.4.2, for details.
#define SEGMENT_SELECTOR(INDEX, TYPE, RPL)                                     \
(                                                                              \
(((INDEX) & 0b111111111111) << 3) |                                            \
(((TYPE) & 0b1) << 2)             |                                            \
 ((RPL) & 0b11)                                                                \
)

#define SEGSEL_TSS        SEGMENT_SELECTOR(SEGSEL_KERNEL_TSS_IDX_LO, 0, 0) // Task Segment Selector
#define SEGSEL_KERNEL_CS  SEGMENT_SELECTOR(SEGSEL_KERNEL_CS_IDX, 0, 0)     // Kernel Code Segment
#define SEGSEL_KERNEL_DS  SEGMENT_SELECTOR(SEGSEL_KERNEL_DS_IDX, 0, 0)     // Kernel Data Segment
#define SEGSEL_USER_CS    SEGMENT_SELECTOR(SEGSEL_USER_CS_IDX, 0, 3)       // User Code Segment
#define SEGSEL_USER_DS    SEGMENT_SELECTOR(SEGSEL_USER_DS_IDX, 0, 3)       // User Data Segment

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_SEG_H
