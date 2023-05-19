// Copyright (C) Fisnik. All rights reserved.
#ifndef STRING_HPP
#define STRING_HPP

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))
#pragma once
#endif

#define STRING_VERSION ""
#define STRING_VERSION_MAJOR 0
#define STRING_VERSION_MINOR 0

#include <stddef.h>    // size_t, ptrdiff_t, etc.
#include <stdarg.h>    // vararg functionality.
#include <stdlib.h>    // malloc, free.
#include <stdexcept>   // std::out_of_range, std::length_error.
#include <cassert>
#include <sstream>
#include <iterator>

namespace detail {
    class string{
    public:
        using iterator = char*;
        using const_iterator = const char*;
        using value_type = char;
        using pointer = char*;
        using const_pointer = char*;
        using reference = char&;
        using const_reference = const char&;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    public:
        string();
        string(const string& rhs);
        string(const std::string& rhs);
        explicit string(const char* cstr);
        string(string&& rhs) noexcept;
        string(std::string&& rhs) noexcept;
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

        static constexpr auto npos          { static_cast<size_t>(-1) };
        size_t size() const noexcept        { return m_size; }
        size_t capacity() const noexcept    { return m_capacity; }
        size_t max_size() const noexcept    { return std::numeric_limits<size_t>::max(); }
        char* data()                        { return m_buffer; }
        const char* data() const            { return m_buffer; }
        const char* c_str() const noexcept;

        char& at(size_t i) noexcept{}
        const char& at(size_t i) const noexcept{}
        string substr(size_t pos = 0, size_t count = npos) const;
        constexpr string substr(size_t pos = 0, size_t count = npos) const;
        constexpr void clear() noexcept;
        void resize(size_t n);
        void push_back(char c){}
        void reserve(const size_t){}
        void shrink_to_fit();
        auto index(const char c) const;
        char& operator[](size_t i) noexcept { return m_buffer[i]; }
        const char& operator[](size_t i) const noexcept { return m_buffer[i]; }

        string& capitalize();
        string& lower();
        string& upper();
        const char& capitalize() const;
        const char& lower() const;
        const char& upper() const;
        
        constexpr bool is_alnum() const noexcept;
        constexpr bool is_lower() const noexcept;
        constexpr bool is_upper() const noexcept;

        constexpr bool contains(const string& s) const noexcept;
        constexpr bool contains(const char* s) const noexcept;

        constexpr bool starts_with(const char* str) const noexcept
        {
            const auto len = length(str);
            auto n = 0, y = 0;
            for(auto iter = cbegin(); iter != cend(); ++iter, ++n){
                if(*iter != str[n]) return false;
                if(*iter == str[n]) ++y;
                if(n == len) break; 
            }
            return (y == len);
        }

        constexpr bool starts_with(const string& str) const noexcept {
            for(auto first = cbegin(), second = str.cbegin();
                first != cend() && second != str.cend(); 
                ++first, ++second)
            {
                if(*first != *second) return false;
            }
        }

        constexpr bool starts_with(const std::string& str) const noexcept {
            auto first = cbegin();
            auto& second = str.cbegin();
            
            for(; first != cend() && second != str.cend(); 
                ++first, ++second)
            {
                if(*first != *second) return false;
            }
        }
        
        constexpr bool ends_with(const string& s) const noexcept;
        
        constexpr bool ends_with(const char* str) const noexcept{
            const auto len = length(str);
            auto n = 0, y = 0;
            for(auto iter = crbegin(); iter != crend(); ++iter, ++n){
                if(*iter != str[n]) return false;
                if(*iter == str[n]) ++y;
                if(n == len) break; 
            }
            return (y == len);
        }

        string& operator=(const string& rhs);
        string& operator+=(const string& rhs);
        string operator+(const string& rhs);
        friend std::ostream& operator<<(std::ostream& out, const string& rhs);
        friend bool operator==(const char* lhs, const string& rhs);
		friend bool operator==(const string& lhs, const char* rhs);
        friend bool operator==(const string& lhs, const string& rhs);
        friend bool operator!=(const string& lhs, const string& rhs);
    private:
        char* m_buffer = nullptr;
        size_t m_size = 0;
        size_t m_capacity = 0;
        void reallocate(size_t n_capacity);
        
        static size_t length(const char* cstr) noexcept 
        {
            if (*cstr == '\0')
			    return 0;

            size_t n = 0;
            while (*(++cstr) != '\0') {
                ++n;
            }
            return n;
        }
    };
}

#endif