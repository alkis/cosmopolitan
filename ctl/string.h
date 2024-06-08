// -*-mode:c++;indent-tabs-mode:nil;c-basic-offset:4;tab-width:8;coding:utf-8-*-
// vi: set et ft=cpp ts=4 sts=4 sw=4 fenc=utf-8 :vi
#ifndef COSMOPOLITAN_CTL_STRING_H_
#define COSMOPOLITAN_CTL_STRING_H_
#include "string_view.h"

namespace ctl {

class string;

string
strcat(const string_view, const string_view) noexcept __wur;

class string
{
  public:
    using iterator = char*;
    using const_iterator = const char*;
    static constexpr size_t npos = -1;

    ~string() /* noexcept */;
    string(const string_view) noexcept;
    string(const char*) noexcept;
    string(const string&) noexcept;
    string(const char*, size_t) noexcept;
    explicit string(size_t, char = 0) noexcept;

    string& operator=(string) noexcept;
    const char* c_str() const noexcept;

    void pop_back() noexcept;
    void grow(size_t) noexcept;
    void reserve(size_t) noexcept;
    void resize(size_t, char = 0) noexcept;
    void append(char) noexcept;
    void append(char, size_t) noexcept;
    void append(unsigned long) noexcept;
    void append(const void*, size_t) noexcept;
    string& insert(size_t, const string_view) noexcept;
    string& erase(size_t = 0, size_t = npos) noexcept;
    string substr(size_t = 0, size_t = npos) const noexcept;
    string& replace(size_t, size_t, const string_view&) noexcept;
    bool operator==(const string_view) const noexcept;
    bool operator!=(const string_view) const noexcept;
    bool contains(const string_view) const noexcept;
    bool ends_with(const string_view) const noexcept;
    bool starts_with(const string_view) const noexcept;
    size_t find(char, size_t = 0) const noexcept;
    size_t find(const string_view, size_t = 0) const noexcept;

    string() noexcept
    {
        set_small_size(0);
#if 0
        small()->buf[0] = 0;
#endif
    }

    void swap(string& s) noexcept
    {
        char tmp[string_size];
        __builtin_memcpy(tmp, __builtin_launder(blob), sizeof(tmp));
        __builtin_memcpy(
          __builtin_launder(blob), __builtin_launder(s.blob), sizeof(tmp));
        __builtin_memcpy(__builtin_launder(s.blob), tmp, sizeof(tmp));
    }

    string(string&& s) noexcept
    {
        __builtin_memcpy(blob, __builtin_launder(s.blob), sizeof(blob));
        s.set_small_size(0);
#if 0
        s.small()->buf[0] = 0;
#endif
    }

    void clear() noexcept
    {
        if (islarge()) {
            large()->n = 0;
        } else {
            set_small_size(0);
        }
    }

    bool empty() const noexcept
    {
        return islarge() ? !large()->n : small()->rem == (sso_max << (is_le ? 0 : 1));
    }

    inline char* data() noexcept
    {
        return islarge() ? large()->p : small()->buf;
    }

    inline const char* data() const noexcept
    {
        return islarge() ? large()->p : small()->buf;
    }

    inline size_t size() const noexcept
    {
#if 0
        if (!islarge() && small()->rem > sso_max)
            __builtin_trap();
#endif
        return islarge() ? large()->n
                         : (sso_max - small()->rem) >> (is_le ? 0 : 1);
    }

    size_t length() const noexcept
    {
        return size();
    }

    size_t capacity() const noexcept
    {
#if 0
        if (islarge() && large()->c <= __::sso_max)
            __builtin_trap();
#endif
        return islarge() ? (is_le ? large()->c & ~msb : large()->c >> 1)
                         : sso_max;
    }

    iterator begin() noexcept
    {
        return data();
    }

    iterator end() noexcept
    {
        return data() + size();
    }

    const_iterator cbegin() const noexcept
    {
        return data();
    }

    const_iterator cend() const noexcept
    {
        return data() + size();
    }

    char& front()
    {
        if (!size())
            __builtin_trap();
        return data()[0];
    }

    const char& front() const
    {
        if (!size())
            __builtin_trap();
        return data()[0];
    }

    char& back()
    {
        if (!size())
            __builtin_trap();
        return data()[size() - 1];
    }

    const char& back() const
    {
        if (!size())
            __builtin_trap();
        return data()[size() - 1];
    }

    char& operator[](size_t i) noexcept
    {
        if (i >= size())
            __builtin_trap();
        return data()[i];
    }

    const char& operator[](size_t i) const noexcept
    {
        if (i >= size())
            __builtin_trap();
        return data()[i];
    }

    void push_back(char ch) noexcept
    {
        append(ch);
    }

    void append(const string_view s) noexcept
    {
        append(s.p, s.n);
    }

    inline operator string_view() const noexcept
    {
        return string_view(data(), size());
    }

    string& operator=(const char* s) noexcept
    {
        clear();
        append(s);
        return *this;
    }

    string& operator=(const string_view s) noexcept
    {
        clear();
        append(s);
        return *this;
    }

    string& operator+=(char x) noexcept
    {
        append(x);
        return *this;
    }

    string& operator+=(const string_view s) noexcept
    {
        append(s);
        return *this;
    }

    string operator+(const string_view s) const noexcept
    {
        return strcat(*this, s);
    }

    int compare(const string_view s) const noexcept
    {
        return strcmp(*this, s);
    }

    bool operator<(const string_view s) const noexcept
    {
        return compare(s) < 0;
    }

    bool operator<=(const string_view s) const noexcept
    {
        return compare(s) <= 0;
    }

    bool operator>(const string_view s) const noexcept
    {
        return compare(s) > 0;
    }

    bool operator>=(const string_view s) const noexcept
    {
        return compare(s) >= 0;
    }

  private:
    static constexpr size_t string_size = 3 * sizeof(size_t);
    static constexpr size_t sso_max = string_size - 1;
    static constexpr size_t msb = ~size_t{0} << (8 * sizeof(size_t) - 1);
    static constexpr bool is_le = __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__;

    struct large_rep
    {
        char* p;
        size_t n;
        // capacity = little_endian ? c & 0x7FFF : c >> 1
        size_t c;
    };
    struct small_rep
    {
        char buf[sso_max];
        // size = little_endian ? sso_max - rem : (sso_max - rem) << 1
        unsigned char rem;
    };

    bool islarge() const noexcept
    {
        return *(__builtin_launder(blob) + sso_max) & (is_le ? 0x80 : 0x01);
    }

    void set_small_size(size_t size) noexcept
    {
        if (size > sso_max)
            __builtin_trap();
        small()->rem = (sso_max - size) << (is_le ? 0 : 1);
    }

    void set_large_capacity(size_t c) noexcept
    {
        if (c & msb)
            __builtin_trap();
        large()->c = is_le ? c | msb : c << 1;
    }

    small_rep* small() noexcept
    {
        if (islarge())
            __builtin_trap();
        return __builtin_launder(reinterpret_cast<small_rep*>(blob));
    }

    const small_rep* small() const noexcept
    {
        if (islarge())
            __builtin_trap();
        return __builtin_launder(
          reinterpret_cast<const small_rep*>(blob));
    }

    large_rep* large() noexcept
    {
        if (!islarge())
            __builtin_trap();
        return __builtin_launder(reinterpret_cast<large_rep*>(blob));
    }

    const large_rep* large() const noexcept
    {
        if (!islarge())
            __builtin_trap();
        return __builtin_launder(reinterpret_cast<const large_rep*>(blob));
    }

    friend string strcat(const string_view, const string_view);

    union {
        small_rep s;
        large_rep l;
        char blob[string_size];
    };
    static_assert(sizeof(string::small_rep) == sizeof(string::large_rep));
    static_assert(sizeof(string::large_rep) == sizeof(string::blob));
};

} // namespace ctl

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wliteral-suffix"
inline ctl::string
operator"" s(const char* s, size_t n)
{
    return ctl::string(s, n);
}
#pragma GCC diagnostic pop

#endif // COSMOPOLITAN_CTL_STRING_H_
