#include "MemoryManager.h"
#include "PoolAllocator.h"
namespace ME
{
	MemoryManager* MemoryManager::Allocator = nullptr;

	void InitAllocator() { MemoryManager::Allocator = new  PoolAllocator<malloc_stdfree_UpstreamMemory>(); }
	void DeInitAllocator() 
	{
		reinterpret_cast<PoolAllocator<malloc_stdfree_UpstreamMemory>*>(MemoryManager::Allocator)->~PoolAllocator();
		delete MemoryManager::Allocator;
	}

	void* malloc_stdfree_UpstreamMemory::allocate(const size_t& size, const char* msg) 
	{
		std::cout << msg << std::endl;
		return malloc(size);
	}
	void* malloc_stdfree_UpstreamMemory::reallocate(void* end_ptr, const size_t& size, const char* msg)
	{
		std::cout << msg << std::endl;
		return ::realloc(end_ptr, size);
	}
	void malloc_stdfree_UpstreamMemory::deallocate(void* ptr, const size_t&, const char* msg) 
	{ 
		if(msg != nullptr)
			std::cout << msg << std::endl;
		std::free(ptr);
	}
	void* alloc_dealloc_UpstreamMemory::allocate(const size_t& size, const char* msg) 
	{
		std::cout << msg << std::endl;
		return (void*)alloc<char>(size);
	}
	void* alloc_dealloc_UpstreamMemory::reallocate(void* end_ptr, const size_t& size, const char* msg)
	{
		std::cout << msg << std::endl;
		return (void*)realloc<char>((char*)end_ptr, size);
	}
	void alloc_dealloc_UpstreamMemory::deallocate(void* end_ptr, const size_t& size, const char* msg) 
	{ 
		std::cout << msg << std::endl;
		dealloc(end_ptr, size);
	}

	MemoryManager::MemoryManager(UpstreamMemory* upstreammemory)
		:m_UpstreamMemory(upstreammemory) {}
}