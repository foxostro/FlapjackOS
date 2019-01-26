#ifdef TESTING
#error "It's not a good idea to try to replace the C++ runtime within the unit test suite. This translation unit is untestable by nature."
#endif

#include <cstdlib>
#include <unwind.h>
#include <common/logger.hpp>

extern "C"
void* __cxa_allocate_exception(size_t thrown_size) throw()
{
    TRACE("thrown_size --> %u", (unsigned)thrown_size);
    return malloc(thrown_size);
}

extern "C"
void __cxa_free_exception(void *thrown_exception) throw()
{
    TRACE("thrown_exception --> %p", thrown_exception);
    free(thrown_exception);
}

extern "C"
void* __cxa_allocate_dependent_exception() throw()
{
    TRACE("stub");
    panic("__cxa_allocate_dependent_exception");
    return nullptr;
}

extern "C"
void __cxa_free_dependent_exception(void* dependent_exception) throw()
{
    TRACE("stub");
    TRACE("dependent_exception --> %p", dependent_exception);
    panic("__cxa_free_dependent_exception");
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
                 struct type_info* tinfo,
                 void (*dest)(void*))
{
    (void)tinfo;
    (void)dest;

    __cxa_exception *header = ((__cxa_exception*)thrown_exception - 1);

    TRACE("about to call _Unwind_RaiseException...");
    _Unwind_RaiseException(&header->unwind_header);
    panic("exception was not handled in __cxa_throw");
}

extern "C"
void* __cxa_get_exception_ptr(void* exception_object) throw()
{
    TRACE("stub");
    TRACE("exception_object --> %p", exception_object);
    panic("__cxa_get_exception_ptr");
    return nullptr;
}

extern "C"
void* __cxa_begin_catch(void* exception_object) throw()
{
    TRACE("stub");
    TRACE("exception_object --> %p", exception_object);
    panic("__cxa_begin_catch");
    return nullptr;
}

extern "C"
void __cxa_end_catch()
{
    TRACE("stub");
    panic("__cxa_end_catch");
}

extern "C"
std::type_info* __cxa_current_exception_type()
{
    TRACE("stub");
    panic("__cxa_current_exception_type");
    return nullptr;
}

extern "C"
void __cxa_rethrow()
{
    TRACE("stub");
    panic("__cxa_rethrow");
}

extern "C"
void* __cxa_current_primary_exception() throw()
{
    TRACE("stub");
    panic("__cxa_current_primary_exception");
}

extern "C"
void __cxa_decrement_exception_refcount(void* primary_exception) throw()
{
    TRACE("stub");
    TRACE("primary_exception --> %p", primary_exception);
    panic("__cxa_decrement_exception_refcount");
}

struct __cxa_eh_globals;
extern "C"
__cxa_eh_globals* __cxa_get_globals() throw()
{
    TRACE("stub");
    panic("__cxa_get_globals");
    return nullptr;
}

extern "C"
__cxa_eh_globals* __cxa_get_globals_fast() throw()
{
    TRACE("stub");
    panic("__cxa_get_globals_fast");
    return nullptr;
}

extern "C"
void __cxa_increment_exception_refcount(void* primary_exception) throw()
{
    TRACE("stub");
    TRACE("primary_exception --> %p", primary_exception);
    panic("__cxa_increment_exception_refcount");
}

extern "C"
void __cxa_rethrow_primary_exception(void* primary_exception)
{
    TRACE("stub");
    TRACE("primary_exception --> %p", primary_exception);
    panic("__cxa_rethrow_primary_exception");
}

extern "C"
bool __cxa_uncaught_exception() throw()
{
    TRACE("stub");
    panic("__cxa_uncaught_exception");
    return false;
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
int __cxa_guard_acquire(uint64_t* guard_object)
{
    TRACE("stub");
    TRACE("guard_object --> %p", guard_object);
    panic("__cxa_guard_acquire");
    return 0;
}

extern "C"
void __cxa_guard_release(uint64_t* guard_object)
{
    TRACE("stub");
    TRACE("guard_object --> %p", guard_object);
    panic("__cxa_guard_release");
}

extern "C"
void __cxa_guard_abort(uint64_t* guard_object)
{
    TRACE("stub");
    TRACE("guard_object --> %p", guard_object);
    panic("__cxa_guard_abort");
}

extern "C"
void* __cxa_vec_new(size_t element_count,
                    size_t element_size,
                    size_t padding_size,
                    void (*constructor)(void*),
                    void (*destructor)(void*))
{
    (void)element_count;
    (void)element_size;
    (void)padding_size;
    (void)constructor;
    (void)destructor;
    TRACE("stub");
    panic("__cxa_vec_new");
    return nullptr;
}

extern "C"
void* __cxa_vec_new2(size_t element_count,
                     size_t element_size,
                     size_t padding_size,
                     void (*constructor)(void*),
                     void (*destructor)(void*),
                     void* (*alloc)(size_t),
                     void (*dealloc)(void*))
{
    (void)element_count;
    (void)element_size;
    (void)padding_size;
    (void)constructor;
    (void)destructor;
    (void)alloc;
    (void)dealloc;
    TRACE("stub");
    panic("__cxa_vec_new2");
    return nullptr;
}

extern "C"
void* __cxa_vec_new3(size_t element_count,
                     size_t element_size,
                     size_t padding_size,
                     void (*constructor)(void*),
                     void (*destructor)(void*),
                     void* (*alloc)(size_t),
                     void (*dealloc)(void*, size_t))
{
    (void)element_count;
    (void)element_size;
    (void)padding_size;
    (void)constructor;
    (void)destructor;
    (void)alloc;
    (void)dealloc;
    TRACE("stub");
    panic("__cxa_vec_new3");
    return nullptr;
}

extern "C"
void __cxa_vec_ctor(void* array_address,
                    size_t element_count,
                    size_t element_size,
                    void (*constructor)(void*),
                    void (*destructor)(void*))
{
    (void)array_address;
    (void)element_count;
    (void)element_size;
    (void)constructor;
    (void)destructor;
    TRACE("stub");
    panic("__cxa_vec_ctor");
}

extern "C"
void __cxa_vec_dtor(void* array_address,
                    size_t element_count,
                    size_t element_size,
                    void (*destructor)(void*))
{
    (void)array_address;
    (void)element_count;
    (void)element_size;
    (void)destructor;
    TRACE("stub");
    panic("__cxa_vec_dtor");
}

extern "C"
void __cxa_vec_cleanup(void* array_address,
                       size_t element_count,
                       size_t element_size,
                       void (*destructor)(void*))
{
    (void)array_address;
    (void)element_count;
    (void)element_size;
    (void)destructor;
    TRACE("stub");
    panic("__cxa_vec_cleanup");
}

extern "C"
void __cxa_vec_delete(void* array_address,
                      size_t element_size,
                      size_t padding_size,
                      void (*destructor)(void*))
{
    (void)array_address;
    (void)element_size;
    (void)padding_size;
    (void)destructor;
    TRACE("stub");
    panic("__cxa_vec_delete");
}

extern "C"
void __cxa_vec_delete2(void* array_address,
                       size_t element_size,
                       size_t padding_size,
                       void (*destructor)(void*),
                       void (*dealloc)(void*))
{
    (void)array_address;
    (void)element_size;
    (void)padding_size;
    (void)destructor;
    (void)dealloc;
    TRACE("stub");
    panic("__cxa_vec_delete2");
}

extern "C"
void __cxa_vec_delete3(void* array_address,
                       size_t element_size,
                       size_t padding_size,
                       void (*destructor)(void*),
                       void (*dealloc)(void*, size_t))
{
    (void)array_address;
    (void)element_size;
    (void)padding_size;
    (void)destructor;
    (void)dealloc;
    TRACE("stub");
    panic("__cxa_vec_delete3");
}

extern "C"
void __cxa_vec_cctor(void* dest_array,
                     void* src_array,
                     size_t element_count,
                     size_t element_size,
                     void (*constructor)(void*, void*),
                     void (*destructor)(void*))
{
    (void)dest_array;
    (void)src_array;
    (void)element_count;
    (void)element_size;
    (void)constructor;
    (void)destructor;
    TRACE("stub");
    panic("__cxa_vec_cctor");
}

extern "C"
void (*__cxa_new_handler)();

extern "C"
void (*__cxa_terminate_handler)();

extern "C"
void (*__cxa_unexpected_handler)();

extern "C" __attribute__((noreturn))
void __cxa_bad_cast()
{
    TRACE("stub");
    panic("__cxa_bad_cast");
}

extern "C" __attribute__((noreturn))
void __cxa_bad_typeid()
{
    TRACE("stub");
    panic("__cxa_bad_typeid");
}

extern "C" __attribute__((noreturn))
void __cxa_pure_virtual(void)
{
    TRACE("stub");
    panic("__cxa_pure_virtual");
}

extern "C" __attribute__((noreturn))
void __cxa_throw_bad_array_new_length(void)
{
    TRACE("stub");
    panic("__cxa_throw_bad_array_new_length");
}

extern "C" __attribute__((noreturn))
void __cxa_call_unexpected(void*)
{
    TRACE("stub");
    panic("__cxa_call_unexpected");
}

extern "C"
char* __cxa_demangle(const char* mangled_name,
                     char* output_buffer,
                     size_t* length,
                     int* status)
{
    (void)mangled_name;
    (void)output_buffer;
    (void)length;
    (void)status;
    TRACE("stub");
    panic("__cxa_demangle");
    return nullptr;
}

namespace __cxxabiv1 {
    struct __class_type_info {
        virtual ~__class_type_info() = default;
    } my__class_type_info;

    struct __si_class_type_info {
        virtual ~__si_class_type_info() = default;
    } my__si_class_type_info;
}

extern "C"
void* __dynamic_cast(const void* __src_ptr,
                     const __cxxabiv1::__class_type_info* __src_type,
                     const __cxxabiv1::__class_type_info* __dst_type,
                     ptrdiff_t __src2dst)
{
    (void)__src_ptr;
    (void)__src_type;
    (void)__dst_type;
    (void)__src2dst;
    TRACE("stub");
    panic("__dynamic_cast");
    return nullptr;
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
