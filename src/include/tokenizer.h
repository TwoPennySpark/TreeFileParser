#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <iostream>
#include <variant>
#include <vector>
#include <regex>

enum TOKEN_TYPE
{
    TOKEN_NULL,
    TOKEN_NAME,
    TOKEN_VALUE,
    TOKEN_LIST_START,
    TOKEN_LIST_END,
    TOKEN_EQUAL_SIGN,
};

/*
 * The Token is a structure that consists of token type and token body
 * For ex. the string: "key = "var"" constists of 5 tokens {TYPE, BODY}:
 * {TOKEN_NAME, "key"}, {TOKEN_NULL, " "}, {TOKEN_EQUAL_SIGN, "="}, {TOKEN_NULL, " "}, {TOKEN_VALUE, ""val""}
 */
using TokenBody = std::string;
using Token = std::pair<TOKEN_TYPE, TokenBody>;

// Extracts tokens from given string through next_token() method
class Tokenizer
{
public:
    Tokenizer(const std::string& s): m_view(s), m_cursor(0) {}

    bool has_more_tokens() const;
    std::optional<Token> next_token();

private:
    std::optional<TokenBody> match(const std::regex& reg);

    std::string_view m_view;
    size_t m_cursor = 0;
};

#endif // TOKENIZER_H
