#include <kprintf.h>

// Maximum number of stack frames in a backtrace.
#define MAXFRAMES (16)

unsigned* get_ebp(); // defined in backtrace_asm.S

void backtrace()
{
	unsigned frame;
	unsigned * ebp;

	ebp = get_ebp();

	kprintf("Back Trace:\n");
	for(frame = 0; frame < MAXFRAMES; ++frame)
	{
		unsigned eip;

		if(ebp == NULL) {
			// We set ebp to zero at the root stack frame.
			break;
		}

		eip = ebp[1];

		if(eip == 0) {
			// No return address?
			break;
		}

		ebp = (unsigned *)(ebp[0]);
		kprintf("[%p]\n", eip);
	}

	kprintf("\n");
}
