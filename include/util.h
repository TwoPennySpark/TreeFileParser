#ifndef _UTIL_H_
#define _UTIL_H_

#include <iostream>
#include <vector>
#include <memory>
#include <string_view>
#include <cstdio>
#include <assert.h>

static size_t find_first_of(std::string_view& s, std::string find, size_t start = 0)
{
    size_t pos = 0;
    if ((pos = s.find_first_of(find, start)) == std::string_view::npos)
        throw std::runtime_error("Invalid string");

    return pos;
}

static size_t find_first_not_of(std::string_view& s, std::string skip, size_t start = 0)
{
    size_t pos = 0;
    if ((pos = s.find_first_not_of(skip, start)) == std::string_view::npos)
        throw std::runtime_error("Invalid string");

    return pos;
}

static size_t check_first_of(std::string_view& s, std::string find, std::string correct, size_t start = 0)
{
    size_t pos = find_first_of(s, find, start);

    for (auto c: correct)
        if (c == s[pos])
            return pos;

    throw std::runtime_error("Invalid string");
}

static size_t check_first_not_of(std::string_view& s, std::string skip, std::string find, size_t start = 0)
{
    size_t pos = find_first_not_of(s, skip, start);

    for (auto c: find)
        if (c == s[pos])
            return pos;

    throw std::runtime_error("Invalid string");
}

static bool check_end(std::string_view& s, std::string skip, size_t start = 0)
{
    if (s.find_first_not_of(skip, start) == std::string_view::npos)
        return true;
    else
        throw std::runtime_error("Invalid string");
}

#endif
