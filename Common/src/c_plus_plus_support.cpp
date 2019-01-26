#ifdef TESTING
#error "It's not a good idea to try to replace the C++ runtime within the unit test suite. This translation unit is untestable by nature."
#endif

#include <cstdlib>
#include <new>
#include <unwind.h>
#include "flapjack_libc/assert.h"
#include <common/logger.hpp>

namespace __cxxabiv1 {
    struct __class_type_info {
        virtual ~__class_type_info() = default;
    } my__class_type_info;

    struct __si_class_type_info {
        virtual ~__si_class_type_info() = default;
    } my__si_class_type_info;
}

extern "C"
void* __cxa_allocate_exception(size_t thrown_size)
{
    TRACE("thrown_size --> %u", (unsigned)thrown_size);
    return nullptr;
}

extern "C"
void __cxa_free_exception(void *thrown_exception)
{
    TRACE("thrown_exception --> %p", thrown_exception);
}

extern "C"
void __cxa_begin_catch()
{
    TRACE("__cxa_begin_catch");
}

extern "C"
void __cxa_end_catch()
{
    TRACE("__cxa_end_catch");
}

using UnexpectedHandler = void(*)(void);
using TerminateHandler = void(*)(void);

struct __cxa_exception { 
	std::type_info* exception_type;
	void (*exception_destructor)(void*);
	UnexpectedHandler unexpected_handler;
	TerminateHandler terminate_handler;
	__cxa_exception* next_exception;

	int handler_count;
	int handler_switch_value;
	const char* action_record;
	const char* language_specific_data;
	void* catch_temp;
	void* adjusted_pointer;

	_Unwind_Exception unwind_header;
};

extern "C"
void __cxa_throw(void* thrown_exception,
                 struct type_info* info,
                 void (*dest)(void*))
{
    TRACE("begin");
    (void)info;
    (void)dest;

    __cxa_exception *header = ((__cxa_exception*)thrown_exception - 1);
    _Unwind_RaiseException(&header->unwind_header);

    TRACE("exception was not handled in __cxa_throw! We must abort now.");
    abort();
}

extern "C"
_Unwind_Reason_Code
__gxx_personality_v0(int version,
                     _Unwind_Action actions,
                     uint64_t exception_class,
                     _Unwind_Exception* unwind_exception,
                     _Unwind_Context* context)
{
    (void)version;
    (void)actions;
    (void)exception_class;
    (void)unwind_exception;
    (void)context;
    TRACE("__gxx_personality_v0 --> _URC_END_OF_STACK");
    return _URC_END_OF_STACK;
}

extern "C"
void __cxa_throw_bad_array_new_length(void)
{
    panic("__cxa_throw_bad_array_new_length");
}

// The C++ runtime needs this function in case a pure virtual function call is
// made. Though, that should not be possible, anyhow.
extern "C" void __cxa_pure_virtual(void)
{
    panic("__cxa_pure_virtual");
}

void* operator new(size_t size)
{
    return malloc(size);
}

void* operator new[](size_t size)
{
    return malloc(size);
}

void* operator new(size_t size, std::align_val_t align)
{
    return memalign(size, static_cast<size_t>(align));
}

void* operator new[](size_t size, std::align_val_t align)
{
    return memalign(size, static_cast<size_t>(align));
}

void operator delete(void* p)
{
    free(p);
}

void operator delete(void* p, size_t)
{
    free(p);
}

void operator delete(void* p, size_t, std::align_val_t)
{
    free(p);
}

void operator delete[](void* p)
{
    free(p);
}

void operator delete[](void* p, size_t)
{
    free(p);
}

void operator delete[](void* p, size_t, std::align_val_t)
{
    free(p);
}

// C++ runtime support for destructors on globals. TODO: Implement these for real.
void *__dso_handle;
extern "C" int __cxa_atexit(void (*)(void *), void *, void *) { return 0; }
extern "C" void __cxa_finalize(void *) {}
