#ifndef BITMAPALLOCATOR
#define BITMAPALLOCATOR

#include <iostream>
#include <memory>
#include <bitset>
#include <mutex>
#include <unordered_map>

#include "MemoryManager.h"

namespace ME
{
    enum PARTITION
    {
       
    };
    template<size_t bytes = 1024ull, size_t partition = 1ull>
    class BitMapAllocator : public MemoryManager
    {
    public:
        BitMapAllocator()
            :head(malloc(bytes)) {}

        BitMapAllocator(const BitMapAllocator&) = delete;
        BitMapAllocator(const BitMapAllocator&& bitmapallocator)
            :m_Position(std::move(bitmapallocator.m_Position)), m_Bitsize(std::move(bitmapallocator.m_Bitsize)), head(bitmapallocator.head)
        {
            bitmapallocator.m_BitMapEntry.m_FreeBits = 0;
            bitmapallocator.m_BitMapEntry.m_Bitmap.fill(true);
            bitmapallocator.head = nullptr;
        }

        BitMapAllocator& operator=(const BitMapAllocator&) = delete;
        BitMapAllocator& operator=(const BitMapAllocator&& bitmapallocator) noexcept
        {
            if (this == &bitmapallocator)
                return *this;

            m_BitMapEntry = std::move(bitmapallocator.m_BitMapEntry);
            m_Position = std::move(bitmapallocator.m_Position);
            head = bitmapallocator.head;

            bitmapallocator.m_BitMapEntry.m_FreeBits = 0;
            bitmapallocator.m_BitMapEntry.m_Bitmap.fill(true);
            bitmapallocator.head = nullptr;

            return *this;
        }
        ~BitMapAllocator()
        {
            std::free(head);
        }

        [[nodiscard]] virtual void* allocate(const size_t& Bytes) override
        {
            if (Bytes)
            {
                std::lock_guard<std::mutex> guard(m_Mutex);

                size_t pos = m_BitMapEntry.GetFreeBits(Bytes);
                if (!m_BitMapEntry.m_Error)
                {
                    m_BitMapEntry.SetBits(pos, true, Bytes);
                    char* result = ((char*)head + pos);
                    m_Position[result] = pos;
                    m_Bitsize[result] = Bytes;
                    return result;
                }
                ME_MEMERROR(nullptr, "Can't Allocate " << Bytes << " bytes, maximum memory available " << m_BitMapEntry.m_FreeBits << " bytes only"
                    << std::endl << "Memory migth have been highly defragmented!!")
            }
            return nullptr;
        }

        [[nodiscard]] virtual void* verified_allocate(void* ptr, const size_t& elementsize, const size_t& elementcount) override
        {
            size_t pos = m_Position[(char*)ptr], bitsize = m_Bitsize[(char*)ptr];
            for (size_t i = pos + bitsize, count = 0; i < bytes; i++)
            {
                if (!m_BitMapEntry.m_Bitmap[i])
                {
                    count++;
                    if (count == elementcount * elementsize)
                    {
                        m_BitMapEntry.SetBits(pos + bitsize, true, elementsize * elementcount);
                        m_Bitsize[(char*)ptr] = bitsize + count;
                        return ptr;
                    }
                }
                else
                {
                    return nullptr;
                }
            }
            return nullptr;
        }

        virtual void deallocate(void* ptr) override
        {

            std::lock_guard<std::mutex> guard(m_Mutex);
            m_BitMapEntry.SetBits(m_Position[(char*)ptr], true, m_Bitsize[(char*)ptr]);
            m_Bitsize.erase((char*)ptr);
            m_Position.erase((char*)ptr);
        }

        virtual size_t getFreeMemory() const override
        {
            return m_BitMapEntry.m_FreeBits;
        }

    private:
        struct BitMapEntry
        {
            bool m_Error;
            size_t m_FreeBits;
            bool m_Bitmap[bytes];

            BitMapEntry()
                :m_FreeBits(bytes), m_Error(false){}

            void SetBits(const size_t& position, const bool& flag, const size_t& count)
            {
                memset(m_Bitmap + position, flag, sizeof(bool) * count);
                if (flag)
                    m_FreeBits -= count;
                else
                    m_FreeBits += count;
            }
            size_t GetFreeBits(const size_t& count)
            {
                size_t start = 0, size = 1;
                bool found = false;
                for (size_t i = 0; i < bytes; i++)
                {
                    if (!m_Bitmap[i] && !found)
                    {
                        start = i;
                        found = true;
                    }

                    if (found)
                    {
                        if (size == count)
                            return start;

                        else if (m_Bitmap[i])
                        {
                            found = false;
                            size = 1;
                        }
                        size++;
                    }
                }
                m_Error = true;
            }
        };

        void* head;
        BitMapEntry m_BitMapEntry;
        std::mutex m_Mutex;
        std::unordered_map<char*, size_t> m_Position;
        std::unordered_map<char*, size_t> m_Bitsize;

        friend class MemoryManager;
    };
}
#endif // !BITMAPALLOCATOR
