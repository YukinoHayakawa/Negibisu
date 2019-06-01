#pragma once

#include <string_view>

enum class TokenType
{
    STRING_LITERAL,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_DOUBLE_BRACE,
    RIGHT_DOUBLE_BRACE,
    COMMA,
    COLON,
    EQUAL,
    NEWLINE,
    END_OF_TOKENS
};

const char * tokenName(TokenType token);

struct Token
{
    int line = 0, column = 0;
    TokenType type = TokenType::END_OF_TOKENS;
    std::string_view text;
};
