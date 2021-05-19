#ifndef MEMORYMANAGER
#define MEMORYMANAGER

#include <memory>
#include <iostream>

#define B * 1
#define KB * 1024
#define MB * 1048576
#define GB * 1073741824

#define ME_MEMMAX 100 KB
#define ME_MEMERROR(condition, msg)\
if(condition){\
std::cout << msg << std::endl;\
throw std::bad_alloc();\
}
#define ME_MEMINIT() InitAllocator()
#define ME_MEMCLEAR() DeInitAllocator()

namespace ME
{
    typedef unsigned long long size_t;

    void InitAllocator();
    void DeInitAllocator();

    class UpstreamMemory
    {
    public:
        virtual void* allocate(const size_t& size, const char* msg = nullptr) = 0;
        virtual void* reallocate(void* end_ptr, const size_t& size, const char* msg = nullptr) = 0;
        virtual void deallocate(void* ptr, const size_t& size, const char* msg = nullptr) = 0;
    };
    class malloc_stdfree_UpstreamMemory : public UpstreamMemory
    {
    public:
        virtual void* allocate(const size_t& size, const char* msg = nullptr) override;
        virtual void* reallocate(void* end_ptr, const size_t& size, const char* msg = nullptr) override;
        virtual void deallocate(void* ptr, const size_t& size, const char* msg = nullptr) override;
    };
    class alloc_dealloc_UpstreamMemory : public UpstreamMemory
    {
    public:
        virtual void* allocate(const size_t& size, const char* msg = nullptr) override;
        virtual void* reallocate(void* end_ptr, const size_t& size, const char* msg = nullptr) override;
        virtual void deallocate(void* ptr, const size_t& size, const char* msg = nullptr) override;
    };
    class null_Upstream : public UpstreamMemory
    {
    public:
        virtual void* allocate(const size_t&, const char* = nullptr) override { ME_MEMERROR(true, "Triggered null_Upstream"); return nullptr; }
        virtual void* reallocate(void* end_ptr, const size_t& size, const char* msg = nullptr) override { allocate(0); }
        virtual void deallocate(void*, const size_t&, const char* = nullptr) override { allocate(0); }
    };

    static UpstreamMemory* set_malloc_stdfree_UpstreamMemory() { return new malloc_stdfree_UpstreamMemory; }
    static UpstreamMemory* set_alloc_dealloc_UpstreamMemory() { return new alloc_dealloc_UpstreamMemory; }
    static UpstreamMemory* set_null_UpstreamMemory() { return new null_Upstream; }

    class MemoryManager
    {
    public:
        UpstreamMemory* m_UpstreamMemory;

        MemoryManager(UpstreamMemory* upstreammemory = set_alloc_dealloc_UpstreamMemory());
        ~MemoryManager() = default;

        [[nodiscard]] virtual void* allocate(const size_t& size) = 0;
        [[nodiscard]] virtual void* verified_allocate(void* end_ptr, const size_t& size) = 0;
        virtual void deallocate(void* ptr, const size_t& size) noexcept = 0;
        virtual void release() = 0;
        virtual size_t getFreeMemory() const = 0;

        // The global Allocator
        static MemoryManager* Allocator;
    };

    // Faster and global Allocators
    // Params Size: Number of variable to be allocated
    template<typename T>
    constexpr static T* alloc(const size_t& size)
    {

        ME_MEMERROR(MemoryManager::Allocator == nullptr, "Allocator not initialized!!");

        if (size)
        {
            return (T*)MemoryManager::Allocator->allocate(sizeof(T) * size);
        }
        return nullptr;
    }
    template<typename T, typename ...Args>
    constexpr static T* allocon(const Args&& ...args)
    {

        ME_MEMERROR(MemoryManager::Allocator == nullptr, "Allocator not initialized!!");

        T* ptr = (T*)MemoryManager::Allocator->allocate(sizeof(T));
        new (ptr) T(args...);
        return ptr;
    }
    template<typename T>
    constexpr static void dealloc(T* ptr, const size_t& size)
    {

        ME_MEMERROR(MemoryManager::Allocator == nullptr, "Allocator not initialized!!");

        MemoryManager::Allocator->deallocate((void*)ptr, size);
    }
    template<typename T>
    constexpr static T* realloc(T* end_ptr, const size_t& size)
    {

        ME_MEMERROR(MemoryManager::Allocator == nullptr, "Allocator not initialized!!");

        return (T*)MemoryManager::Allocator->verified_allocate(end_ptr, sizeof(T) * size);
    }
}

#endif // !MEMORYMANAGER