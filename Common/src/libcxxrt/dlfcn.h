#ifndef FLAPJACKOS_COMMON_INCLUDE_DLFCN_H
#define FLAPJACKOS_COMMON_INCLUDE_DLFCN_H

// Minimal dlfcn stubs so that libcxxrt will build.

#ifdef __cplusplus
extern "C" {
#endif

void *dlopen(const char *filename, int flags);
void *dlsym(void *handle, const char *name);
int dlclose(void *handle);
char *dlerror(void);

typedef struct {
    const char *dli_fname; // Pathname of shared object that contains address
    void       *dli_fbase; // Base address at which shared object is loaded
    const char *dli_sname; // Name of symbol whose definition overlaps addr
    void       *dli_saddr; // Exact address of symbol named in dli_sname
} Dl_info;

int dladdr(void *addr, Dl_info *info);

#ifdef __cplusplus
}
#endif

#endif // FLAPJACKOS_COMMON_INCLUDE_DLFCN_H