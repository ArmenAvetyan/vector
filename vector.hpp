#pragma once
#include <iostream>
#include <cstddef>
#include <memory>
#include <utility>
#include <cstring>
#include <print>
#include <initializer_list>

template<typename T>
class vector {
    private:
        size_t m_size {};
        size_t m_capacity {};
        std::unique_ptr<T[]> m_ptr {};
        inline auto m_alloc() 
        {
            m_capacity = m_capacity == 0 ? 1 : m_capacity * 2;
            return std::make_unique<T[]>(m_capacity);
        }

    public:
        void clear() { m_size = 0; }

        size_t insert(size_t pos, const T& val)
        {
            if (pos > m_size)
                pos = m_size;

            if (m_size == m_capacity)
            {
                auto new_ptr = m_alloc();

                for (size_t i = 0; i < pos; ++i)
                    new_ptr[i] = std::move(m_ptr[i]);

                new_ptr[pos] = val;

                for (size_t i = pos; i < m_size; ++i)
                    new_ptr[i + 1] = std::move(m_ptr[i]);

                m_ptr = std::move(new_ptr);
            } else
            {
                for (size_t i = m_size; i > pos; --i)
                    m_ptr[i] = m_ptr[i - 1];

                m_ptr[pos] = val;
            }

            ++m_size;
            return pos;
        }

        size_t insert(size_t pos, size_t count, const T& val)
        {
            if (pos > m_size)
            pos = m_size;
            if (count == 0)
            return pos;

            if (m_size + count > m_capacity)
            {
                auto new_ptr = std::make_unique<T[]>((m_size + count) * 2);
                m_capacity = (m_size + count) * 2;

                for (size_t i = 0; i < pos; ++i)
                    new_ptr[i] = std::move(m_ptr[i]);

                for (size_t i = 0; i < count; ++i)
                    new_ptr[pos + i] = val;

                for (size_t i = pos; i < m_size; ++i)
                    new_ptr[i + count] = std::move(m_ptr[i]);
    
                m_ptr = std::move(new_ptr);
            } else 
            {
                for (size_t i = m_size; i > pos; --i)
                    m_ptr[i - 1 + count] = m_ptr[i - 1];

                for (size_t i = 0; i < count; ++i)
                    m_ptr[pos + i] = val;
            }

            m_size += count;
            return pos;
        }

        size_t insert(size_t pos, std::initializer_list<T> ilist)
        {
            if(pos > m_size)
                pos = m_size;
            if(ilist.size() == 0)
                return pos;

            size_t count = ilist.size();

            if (m_size + count > m_capacity)
            {
                auto new_ptr = std::make_unique<T[]>((m_size + count) * 2);
                m_capacity = (m_size + count) * 2;

                for (size_t i = 0; i < pos; ++i)
                    new_ptr[i] = std::move(m_ptr[i]);

                size_t idx = 0;
                for (const T& x : ilist) {
                    new_ptr[pos + idx] = x;
                    ++idx;
                }

                for (size_t i = pos; i < m_size; ++i)
                    new_ptr[i + count] = std::move(m_ptr[i]);

                m_ptr = std::move(new_ptr);
            } else
            {
                for (size_t i = m_size; i > pos; --i)
                    m_ptr[i - 1 + count] = m_ptr[i - 1];

                size_t idx = 0;
                for (const T& x : ilist) {
                    m_ptr[pos + idx] = x;
                    ++idx;
                }
            }
            m_size += count;
            return pos;
        }

        size_t erase(size_t pos)
        {
            if (pos >= m_size)
                return m_size;

            for (size_t i = pos; i + 1 < m_size; ++i)
                m_ptr[i] = m_ptr[i + 1];

            --m_size;
            return pos;
        }

        size_t erase(size_t first, size_t last)
        {
            if (first >= m_size || last > m_size || first >= last)
                return m_size;

            size_t count = last - first;

            for (size_t i = first; i < m_size - count; ++i)
                m_ptr[i] = m_ptr[i + count];

            m_size -= count;
            return first;
        }

        void push_back(const T& val)
        {
            if (m_size == m_capacity) 
            {
                auto new_ptr = m_alloc();
    
                for (size_t i = 0; i < m_size; ++i)
                    new_ptr[i] = std::move(m_ptr[i]);

                m_ptr = std::move(new_ptr);
            }

            m_ptr[m_size++] = val;
        }

        void pop_back()
        {
            if (m_size == 0) return;
            --m_size;
        }

        void resize(size_t count, const T& val = T())
        {
            if (count < m_size)
            {
                m_size = count;
                return;
            }

            if (count > m_capacity)
            {
                auto new_ptr = std::make_unique<T[]>(count * 2);

                for (size_t i = 0; i < m_size; ++i)
                    new_ptr[i] = std::move(m_ptr[i]);

                m_ptr = std::move(new_ptr);
            }

            for (size_t i = m_size; i < count; ++i)
                m_ptr[i] = val;

            m_size = count;
        }

        void swap(vector& other)
        {
            std::swap(m_ptr, other.m_ptr);
            std::swap(m_size, other.m_size);
            std::swap(m_capacity, other.m_capacity);
        }

        void print() const
        {
            for(size_t i = 0; i < m_size; ++i)
                std::cout << m_ptr[i] << ' ';

            std::cout << std::endl;
        }

        size_t size() const { return m_size; }

        size_t capacity() const { return m_capacity; }

        bool empty() const { return m_size == 0; }
};

template <>
class vector<bool>
{
    private:
        size_t m_size {};
        size_t m_capacity {};
        unsigned char* m_ptr {};

        void realloc(size_t new_bit_cap)
        {
            size_t new_byte_cap = (new_bit_cap + 7) / 8;
            unsigned char* new_ptr = new unsigned char[new_byte_cap];
            std::memset(new_ptr, 0, new_byte_cap);

            if(m_ptr)
            {
                size_t old_bytes = (m_size + 7) / 8;
                std::memcpy(new_ptr, m_ptr, old_bytes);
                delete[] m_ptr;
            }

            m_ptr = new_ptr;
            m_capacity = new_bit_cap;
        }

    public:
        vector() = default;

        explicit vector(size_t size) : m_size{size}, m_capacity{size}
        {
            if(size > 0)
                realloc(size);
        }

        vector(const vector& other) : m_size{other.m_size}, m_capacity{other.m_size}
        {
            if(other.m_size > 0)
            {
                realloc(other.m_size);
                size_t bytes = (other.m_size + 7) / 8;
                std::memcpy(m_ptr, other.m_ptr, bytes);
            }
        }

        ~vector() { delete[] m_ptr; }

    public:
        size_t size() const { return m_size; }

        size_t capacity() const { return m_capacity; }

        class reference
        {
            private:
                unsigned char& m_byte;
                unsigned char m_mask;

            public:
                reference(unsigned char& byte, size_t bit_pos)
                    : m_byte{byte}, m_mask{static_cast<unsigned char>(1u << (bit_pos % 8))}
                {}

                reference& operator=(bool value)
                {
                    if(value)
                        m_byte |= m_mask;
                    else
                        m_byte &= ~m_mask;
                    return *this;
                }

                operator bool() const noexcept
                {
                    return (m_byte & m_mask) != 0;
                }
        };

        reference operator[](size_t index)
        {
            if(index >= m_size)
                throw std::out_of_range("out of range");

            size_t byte_n = index / 8;
            return reference(m_ptr[byte_n], index);
        }

        bool operator[](size_t index) const
        {
            if(index >= m_size)
                throw std::out_of_range("out of range");

            size_t byte_n = index / 8;
            unsigned char mask = static_cast<unsigned char>(1u << (index % 8));
            return (m_ptr[byte_n] & mask) != 0;
        }

        void push_back(bool value)
        {
            if(m_size == m_capacity)
            {
                size_t new_cap = (m_capacity == 0) ? 64 : m_capacity * 2;
                realloc(new_cap);
            }
            
            size_t byte_n = m_size / 8;
            unsigned char mask = static_cast<unsigned char>(1u << (m_size % 8));

            if(value)
                m_ptr[byte_n] |= mask;
            
            ++m_size;
        }

        void print() const
        {
            for(int i = 0; i < m_size; ++i)
                std::print("{}", ((*this)[i] ? 1 : 0));
        
            std::println();
        }
};
