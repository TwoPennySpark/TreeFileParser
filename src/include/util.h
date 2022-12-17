#ifndef UTIL_H
#define UTIL_H

#include <iostream>

const std::string WHITESPACE_CHARS = " \t\r\n";

// move view starting position 'n' chars forward
static inline void shift_view(std::string_view& s, const size_t n)
{
    s = s.substr(n);
}

// find first occurrence of 'find' characters, starting from 'start'
static size_t find_first_of(const std::string_view& s, const std::string find, const size_t start = 0)
{
    size_t pos = 0;
    if ((pos = s.find_first_of(find, start)) == std::string_view::npos)
        throw std::runtime_error("No symbol was found");

    return pos;
}

// find first absence of 'skip' characters, starting from 'start'
static size_t find_first_not_of(const std::string_view& s, const std::string skip, const size_t start = 0)
{
    size_t pos = 0;
    if ((pos = s.find_first_not_of(skip, start)) == std::string_view::npos)
        throw std::runtime_error("No symbol was found");

    return pos;
}

// find first occurrence of 'possible' characters, then check that it is one of those from 'expected'
static size_t check_first_of(const std::string_view& s,  const std::string possible,
                             const std::string expected, const size_t start = 0)
{
    size_t pos = find_first_of(s, possible, start);

    if (expected.find(s[pos]) != std::string::npos)
        return pos;

    throw std::runtime_error("Unexpected symbol");
}

// find first absence of 'skip' characters, then check that it is one of those from 'find'
static size_t check_first_not_of(const std::string_view& s, const std::string skip,
                                 const std::string find, const size_t start = 0)
{
    size_t pos = find_first_not_of(s, skip, start);

    if (find.find(s[pos]) != std::string::npos)
        return pos;

    throw std::runtime_error("Unexpected symbol");
}

// look for 'until' char, then shift to pos behind it
// if any other non-whitespace chars is found - throw
static void skip_until(std::string_view& s, const char until)
{
    auto pos = check_first_not_of(s, WHITESPACE_CHARS, {until});

    shift_view(s, pos+1);
}

// throw if there are some non-whitespace chars between current position and end
static void check_any_left(const std::string_view& s, const size_t start = 0)
{
    if (s.find_first_not_of(WHITESPACE_CHARS, start) != std::string_view::npos)
        throw std::runtime_error("Unexpected symbol at the end");
}

#endif
