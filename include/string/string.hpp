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

namespace detail {
    class string{
    public:
        using iterator = char*;
		using const_iterator = const char*;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    public:
        string();
        string(const string& rhs);
        explicit string(const char* cstr);
        string(string&& rhs) noexcept;
        string(std::initializer_list<char> list);
        ~string();

        iterator begin();
        iterator end();
        const_iterator cbegin() const;
        const_iterator cend() const;
        reverse_iterator rbegin();
        const_reverse_iterator crbegin() const noexcept;
        reverse_iterator rend();
        const_reverse_iterator crend() const noexcept;

        static constexpr auto npos          { static_cast<size_t>(-1) };
        size_t size() const noexcept        { return m_size; }
        size_t capacity() const noexcept    { return m_capacity; }
        size_t max_size() const noexcept    { return std::numeric_limits<size_t>::max(); }
        char* data()                        { return m_str; }
        const char* data() const            { return m_str; }
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
        char& operator[](size_t i) noexcept { return m_str[i]; }
        const char& operator[](size_t i) const noexcept { return m_str[i]; }

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
        constexpr bool starts_with(const string& s) const noexcept;
        constexpr bool starts_with(const char* s) const noexcept;
        constexpr bool ends_with(const string& s) const noexcept;
        constexpr bool ends_with(const char* s) const noexcept;

        string& operator=(const string& rhs);
        string& operator+=(const string& rhs);
        string operator+(const string& rhs);
        friend std::ostream& operator<<(std::ostream& out, const string& rhs);
        friend bool operator==(const char* lhs, const string& rhs);
		friend bool operator==(const string& lhs, const char* rhs);
        friend bool operator==(const string& lhs, const string& rhs);
        friend bool operator!=(const string& lhs, const string& rhs);
    private:
        char* m_str = nullptr;
        size_t m_size = 0;
        size_t m_capacity = 0;
        void reallocate(size_t n_capacity);
        static size_t length(const char* cstr) noexcept;
    };
}

#endif