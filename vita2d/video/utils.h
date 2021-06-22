#ifndef UTILS_H
#define UTILS_H

#include <psp2/kernel/sysmem.h> 
#include <psp2/gxm.h>

#include <stdlib.h>
#include <malloc.h>
#include <cstdint>
#include <cstring>

#define ALIGN(x, a)	((((unsigned int)x)+((a)-1u))&(~((a)-1u)))

void* allocate_gpu (void* arg, uint32_t alignment, uint32_t size);
void deallocate_gpu (void* jumpback, void* ptr);
void* allocate (void* arg, uint32_t alignment, uint32_t size);
void deallocate (void* arg, void* ptr);

#endif
