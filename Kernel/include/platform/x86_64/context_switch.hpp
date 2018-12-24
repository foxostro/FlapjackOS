#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_CONTEXT_SWITCH_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_CONTEXT_SWITCH_HPP

extern "C" {
void x86_64_context_switch(char** old_stack_pointer, char* new_stack_pointer);
}

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_CONTEXT_SWITCH_HPP