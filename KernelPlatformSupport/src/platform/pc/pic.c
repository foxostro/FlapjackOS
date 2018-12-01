// See <http://wiki.osdev.org/PIC> for information about the PIC.
#include <platform/pc/pic.h>
#include <inout.h>

#define PIC1           0x20 // IO base address for master PIC.
#define PIC2           0xA0 // IO base address for slave PIC.
#define PIC1_COMMAND   PIC1
#define PIC1_DATA      (PIC1+1)
#define PIC2_COMMAND   PIC2
#define PIC2_DATA      (PIC2+1)

#define ICW1_ICW4       0x01 // ICW4 (not) needed
#define ICW1_SINGLE     0x02 // Single (cascade) mode
#define ICW1_INTERVAL4  0x04 // Call address interval 4 (8)
#define ICW1_LEVEL      0x08 // Level triggered (edge) mode
#define ICW1_INIT       0x10 // Initialization - required!
 
#define ICW4_8086       0x01 // 8086/88 (MCS-80/85) mode
#define ICW4_AUTO       0x02 // Auto (normal) EOI
#define ICW4_BUF_SLAVE  0x08 // Buffered mode/slave
#define ICW4_BUF_MASTER 0x0C // Buffered mode/master
#define ICW4_SFNM       0x10 // Special fully nested (not)

#define PIC_READ_ISR    0x0b
#define PIC_EOI         0x20 // Command code for "end of interrupt"

void pic_remap(unsigned char master_base, unsigned char slave_base)
{
    unsigned char a1 = inb(PIC1_DATA);
    unsigned char a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT+ICW1_ICW4);
    outb(PIC1_DATA, master_base);
    outb(PIC1_DATA, 4);
    outb(PIC1_DATA, ICW4_8086);

    outb(PIC2_COMMAND, ICW1_INIT+ICW1_ICW4);
    outb(PIC2_DATA, slave_base);
    outb(PIC2_DATA, 2);
    outb(PIC2_DATA, ICW4_8086);

    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}

void pic_init(void)
{
    // In protected mode, the IRQs 0 to 7 conflict with the CPU exceptions which
    // are reserved by Intel up until 0x1F. Remap them.
    pic_remap(0x20, 0x28);
}
 
static uint16_t pic_get_isr(void)
{
    outb(PIC1_COMMAND, PIC_READ_ISR);
    outb(PIC2_COMMAND, PIC_READ_ISR);
    return (inb(PIC2_COMMAND) << 8) | inb(PIC1_COMMAND);
}

bool pic_clear(unsigned interrupt_number)
{
    unsigned irq = interrupt_number - 0x20;

    uint16_t isr = pic_get_isr();

    if ((irq == 7) && ((isr & (1 << irq)) == 0)) {
        return true;
    }

    if (irq >= 8) {
        outb(PIC2, PIC_EOI);    
    }

    outb(PIC1, PIC_EOI);

    return false;
}
