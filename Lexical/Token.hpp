#pragma once

#include <charconv>
#include <typeinfo>
#include <iosfwd>

#include "SourcePosition.hpp"

namespace usagi::negi
{
enum class TokenType
{
    SHARP,
    NEWLINE,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_DOUBLE_BRACE,
    RIGHT_DOUBLE_BRACE,
    STRING_LITERAL,
    COLON,
    COMMA,
    EQUAL,
    DOUBLE_QUOTE,
    END_OF_STREAM,
    UNKNOWN,
};

const char * tokenName(TokenType token);
const char * tokenSymbol(TokenType token);

struct Token
{
    std::size_t index = -1;
    SourcePosition pos;
    TokenType type = TokenType::UNKNOWN;
    std::string_view text;

    template <typename T>
    auto as()
    {
        T result;
        // https://en.cppreference.com/w/cpp/utility/from_chars
        if(auto [p, ec] = std::from_chars(
                text.data(), text.data() + text.size(), result);
            ec == std::errc())
            return result;
        throw std::bad_cast();
    }

    template <typename T>
    bool convertibleTo() const
    {
        T result;
        auto [p, ec] = std::from_chars(
            text.data(), text.data() + text.size(), result
        );
        return ec == std::errc();
    }
};

struct TokenRef
{
    const Token *ref = nullptr;

    const Token *operator->() const
    {
        return ref;
    }

    operator bool() const
    {
        return ref;
    }

    operator const Token *() const
    {
        return ref;
    }
};

std::ostream & operator<<(std::ostream &os, const Token &t);
// checks iterator validity before output
std::ostream & operator<<(std::ostream &os, const TokenRef &t);
}
