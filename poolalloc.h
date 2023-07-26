#pragma once

#include <iostream>
#include <memory>
#include <vector>

#define UNUSED(variable) (void)variable

template <class T, size_t m_chunks_per_block>
struct pool_allocator {
    using value_type = T;

    struct chunk {
        chunk *next;
    };

    pool_allocator () noexcept {}

    ~pool_allocator() {
        for (auto block : v_block_address) {
            //std::cout << "free = " << m_chunks_per_block*sizeof(T) << std::endl;
            free(block);
        }
    }

    template <class U> pool_allocator(const pool_allocator<U,m_chunks_per_block>&) noexcept {}

    T* allocate (std::size_t n)
    {
        if( n != 1 ) throw std::bad_alloc();              // выделять за раз не больше одного елемента

        if (m_alloc == nullptr) {
            m_alloc = allocate_block(n * sizeof(T));
            v_block_address.push_back(m_alloc);
        }

        //std::cout << "allocate = " << n*sizeof(T) << std::endl;

        chunk *free_chunk = m_alloc;

        m_alloc = m_alloc->next;

        return reinterpret_cast<T*>(free_chunk);

    }
    void deallocate (T* ptr, std::size_t n)
    {
        //std::cout << "dealocate = " << n*sizeof(T) << std::endl;
        reinterpret_cast<chunk *>( ptr)->next = m_alloc;
        m_alloc = reinterpret_cast<chunk*>( ptr);
        UNUSED(n);	
    }

    template< class U >
    struct rebind
    {
        typedef pool_allocator<U, m_chunks_per_block> other;
    };

private:
    chunk* m_alloc = nullptr;

    std::vector<chunk*>v_block_address;

    chunk* allocate_block(size_t chunk_size){
        size_t block_size  = m_chunks_per_block * chunk_size;

        //std::cout << "malloc = " << block_size << std::endl;

        chunk* block_begin = reinterpret_cast<chunk*>(malloc(block_size));

        // После выделения памяти для блока, инициализируем цепочку элементов в этом блоке
        chunk* ptr = block_begin;

        for (size_t i = 0; i < m_chunks_per_block - 1; ++i) {
            ptr->next = reinterpret_cast<chunk*>(reinterpret_cast<char *>(ptr) + chunk_size);
            ptr = ptr->next;
        }

        ptr->next = nullptr;

        return block_begin;
    };
};

template <class T, class U, size_t N>
constexpr bool operator== (const pool_allocator<T,N>& a1, const pool_allocator<U,N>& a2) noexcept
{
    return true;
}

template <class T, class U, size_t N>
constexpr bool operator!= (const pool_allocator<T, N>& a1, const pool_allocator<U, N>& a2) noexcept
{
    return false;
}

