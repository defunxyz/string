// Copyright (C) Fisnik. All rights reserved.
#ifndef STRING_HPP
#define STRING_HPP

#define STRING_VERSION ""
#define STRING_VERSION_MAJOR 0
#define STRING_VERSION_MINOR 0

#include <stddef.h>  // size_t, ptrdiff_t, etc.
#include <stdexcept> // std::out_of_range, std::length_error.
#include <cstddef>
#include <sstream>
#include <iterator>

namespace detail
{
    class string
    {
        template<typename E, typename... Args>
        inline static void static_enforce(bool condition, Args&&... args){
            if(condition){
                throw_exception(E(static_cast<Args&&>(args)...));
            }
        }

    public:
        using value_type = char;
        using pointer = char*;
        using const_pointer = const char*;
        using reference = char&;
        using const_reference = const char&;
        
        using iterator = pointer;
        using const_iterator = const_pointer;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    public:
        string() 
            : string("") 
        {}

        string(const string& str) 
            : m_size{0}, m_capacity{0}
        {
            m_size = str.size();
            m_capacity = str.capacity();
            m_buffer = new char[m_capacity];
            std::copy(str.cbegin(), str.cend(), m_buffer);
        }

        string(const std::string &rhs);
        
        explicit string(const char *str)
            : m_size{0}, m_capacity{0}
        {
            m_size = length_helper(str);
		    m_capacity = m_size + 1;
		    m_buffer = new char[m_capacity];
		    std::copy(str, str + m_size, m_buffer);
        }

        string(string &&rhs) noexcept;
        string(std::string &&rhs) noexcept;
        string(const_pointer, const size_t);
        string(std::initializer_list<char> list);
        ~string();

        iterator begin() noexcept { return m_buffer; };
        iterator end() noexcept { return (m_buffer + m_size); };
        const_iterator cbegin() const noexcept { return m_buffer; };
        const_iterator cend() const noexcept { return m_buffer + m_size; };

        reverse_iterator rbegin() { return reverse_iterator(end()); }
        reverse_iterator rend() { return reverse_iterator(begin()); }
        const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }
        const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

        static constexpr auto npos{static_cast<size_t>(-1)};
        size_t size() const noexcept { return m_size; }
        size_t length() const noexcept { return m_size; }
        size_t capacity() const noexcept { return m_capacity; }
        size_t max_size() const noexcept { return std::numeric_limits<size_t>::max(); }
        pointer data() { return m_buffer; }
        const_pointer data() const { return m_buffer; }
        const_pointer c_str() const noexcept { return m_buffer; };

        reference operator[](size_t pos) { return *(begin() + pos); }
        const_reference operator[](size_t pos) const { return *(cbegin() + pos); } 
        
        reference at(size_t n) noexcept {
            static_enforce<std::out_of_range>((n < m_size), 
            "Out Of Range Exception: Requested array index does not fall within the array range.");
            return (*this)[n];
        }

        const_reference at(size_t n) const noexcept {
            static_enforce<std::out_of_range>((n < m_size), 
            "Out Of Range Exception: Requested array index does not fall within the array range.");
            return (*this)[n];
        }
        
        string substr(size_t pos = 0, size_t count = npos) const& {
            static_enforce<std::out_of_range>(pos <= size(), "");
            return string(data() + pos, std::min(count, size() - pos));
        }

        string substr(size_t pos = 0, size_t count = npos) && {};
        void resize(size_t n);
        void push_back(char c) {}
        void reserve(const size_t) {}
        void shrink_to_fit();
        void clear() noexcept { resize(0); }
        bool empty() const { return size() == 0; }
        
        int index(const char ch) const
        {
            int pos = 0;
            for(auto iter = cbegin(); iter != cend(); ++iter, ++pos)
                if(*iter == ch)
                    return pos;
            
            return npos;
        }
        
        char &operator[](size_t i) noexcept { return m_buffer[i]; }
        const char &operator[](size_t i) const noexcept { return m_buffer[i]; }

        void swap(string& rhs) noexcept;

        string& capitalize();
        string& lower();
        string& upper();
        const_reference capitalize() const;
        const_reference lower() const;
        const_reference upper() const;

        constexpr bool is_alnum() const noexcept;
        constexpr bool is_lower() const noexcept;
        constexpr bool is_upper() const noexcept;
        
        constexpr bool compare(const string& str)
        {
            if(str.size() != size() || str.capacity() != capacity())
                return false;
            
            for(auto first = cbegin(), second = str.cbegin(); 
            first != cend() && second != str.cend(); 
            ++first, ++second){
                if(*first != *second)
                    return false;
            }

            return true;
        }

        constexpr bool contains(const string& str) const noexcept;
        constexpr bool contains(const char *str) const noexcept;

        constexpr bool starts_with(const char *str) const noexcept
        {
            const auto len = length_helper(str);
            auto n = 0, y = 0;
            for (auto iter = cbegin(); iter != cend(); ++iter, ++n)
            {
                if (*iter != str[n])
                    return false;
                if (*iter == str[n])
                    ++y;
                if (n == len)
                    break;
            }
            return (y == len);
        }

        constexpr bool starts_with(const string &str) const noexcept
        {
            for (auto first = cbegin(), second = str.cbegin();
                 first != cend() && second != str.cend();
                 ++first, ++second)
            {
                if (*first != *second)
                    return false;
            }
        }

        constexpr bool starts_with(const std::string &str) const noexcept
        {
            auto first = cbegin();
            auto &second = str.cbegin();

            for (; first != cend() && second != str.cend();
                 ++first, ++second)
            {
                if (*first != *second)
                    return false;
            }
        }

        constexpr bool ends_with(const string &str) const noexcept 
        {
            auto first = crbegin();
            auto second = str.cbegin();

            for (;first != crend() && second != str.cend();
                 ++first, ++second)
            {
                if (*first != *second)
                    return false;
            }
        }

        constexpr bool ends_with(const std::string &str) const noexcept
        {
            auto first = crbegin();
            auto &second = str.cbegin();

            for (; first != crend() && second != str.cend();
                 ++first, ++second)
            {
                if (*first != *second)
                    return false;
            }
        }

        constexpr bool ends_with(const char *str) const noexcept
        {
            const auto len = length_helper(str);
            auto n = 0, y = 0;
            for (auto iter = crbegin(); iter != crend(); ++iter, ++n)
            {
                if (*iter != str[n])
                    return false;
                if (*iter == str[n])
                    ++y;
                if (n == len)
                    break;
            }
            return (y == len);
        }

        string& operator=(const string &rhs);
        string& operator=(string&& rhs);
        string& operator+=(const string &rhs);
        string operator+(const string &rhs);
        
        friend void swap(string& lhs, string& rhs);
        friend std::ostream &operator<<(std::ostream &out, const string &rhs);
        friend bool operator==(const char *lhs, const string &rhs);
        friend bool operator==(const string &lhs, const char *rhs);
        friend bool operator==(const string &lhs, const string &rhs);
        friend bool operator!=(const string &lhs, const string &rhs);

    private:
        size_t m_size;
        size_t m_capacity;
        char *m_buffer;
        
        void reallocate(size_t n_capacity);
        static size_t length_helper(const char *cstr) noexcept
        {
            if (*cstr == '\0')
                return 0;

            size_t n = 0;
            while (*(++cstr) != '\0') ++n;

            return n;
        }
    };
}

#endif
