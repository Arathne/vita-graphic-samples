#include "utils.h"

void* allocate (void* arg, uint32_t alignment, uint32_t size)
{
	return memalign(alignment, size);
}

void deallocate (void* arg, void* ptr)
{
	free(ptr);
}

void* allocate_gpu (void* arg, uint32_t alignment, uint32_t size)
{
	void* ptr = nullptr;

	if (alignment < 0x40000)
	{
		alignment = 0x40000;
	}

	size = ALIGN(size, alignment);

	SceKernelAllocMemBlockOpt opt;
	memset(&opt, 0, sizeof(opt));
	opt.size = sizeof(SceKernelAllocMemBlockOpt);
	opt.attr = SCE_KERNEL_ALLOC_MEMBLOCK_ATTR_HAS_ALIGNMENT;
	opt.alignment = alignment;

	SceUID uid = sceKernelAllocMemBlock("Video Memblock", SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW, size, &opt);
	
	sceKernelGetMemBlockBase(uid, &ptr);
	sceGxmMapMemory(ptr, size, (SceGxmMemoryAttribFlags)(SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE));
	
	return ptr;
}

void deallocate_gpu (void* jumpback, void* ptr)
{
	SceUID uid = sceKernelFindMemBlockByAddr(ptr, 0);
	sceGxmUnmapMemory(ptr);
	sceKernelFreeMemBlock(uid);
}
