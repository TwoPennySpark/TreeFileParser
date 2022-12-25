#include "tokenizer.h"

const std::vector<std::pair<std::regex, TOKEN_TYPE>> TokenSpecs =
{
    {std::regex("^\\s+"),                   TOKEN_NULL      },
    {std::regex("^[a-zA-Z_][a-zA-Z0-9_]*"), TOKEN_NAME      },
    {std::regex(R"(^"[^"\n]*")"),           TOKEN_VALUE     },
    {std::regex("^[=]"),                    TOKEN_EQUAL_SIGN},
    {std::regex("^[{]"),                    TOKEN_LIST_START},
    {std::regex("^[}]"),                    TOKEN_LIST_END  },
};

bool Tokenizer::has_more_tokens() const
{
    return m_cursor < m_view.size();
}

std::optional<Token> Tokenizer::next_token()
{
    if (!has_more_tokens())
        return std::nullopt;

    // use regular expressions in order to extract next token
    for (auto& [spec, type]: TokenSpecs)
    {
        if (auto res = match(spec); res)
        {
            // skip whitespaces
            if (type == TOKEN_NULL)
                return next_token();

            return Token(type, *res);
        }
    }

    // no match found
    return std::nullopt;
}

std::optional<TokenBody> Tokenizer::match(const std::regex& reg)
{
    std::match_results<decltype(m_view)::const_iterator> res;
    if (std::regex_search(m_view.cbegin() + m_cursor, res, reg))
    {
        TokenBody match = res[0].str();
        m_cursor += match.length();
        return match;
    }

    return std::nullopt;
}
