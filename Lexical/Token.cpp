#include "Token.hpp"

#include <Usagi/Core/Format.hpp>

namespace usagi::negi
{
const char * tokenName(const TokenType token)
{
    switch(token)
    {
        case TokenType::SHARP: return "SHARP";
        case TokenType::NEWLINE: return "NEWLINE";
        case TokenType::LEFT_BRACKET: return "LEFT_BRACKET";
        case TokenType::RIGHT_BRACKET: return "RIGHT_BRACKET";
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::LEFT_DOUBLE_BRACE: return "LEFT_DOUBLE_BRACE";
        case TokenType::RIGHT_DOUBLE_BRACE: return "RIGHT_DOUBLE_BRACE";
        case TokenType::STRING_LITERAL: return "STRING_LITERAL";
        case TokenType::COMMA: return "COMMA";
        case TokenType::COLON: return "COLON";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::END_OF_STREAM: return "END_OF_STREAM";
        case TokenType::DOUBLE_QUOTE: return "DOUBLE_QUOTE";
        default: return "UNKNOWN";
    }
}

const char * tokenSymbol(const TokenType token)
{
    switch(token)
    {
        case TokenType::SHARP: return "#";
        case TokenType::NEWLINE: return "NEWLINE";
        case TokenType::LEFT_BRACKET: return "[";
        case TokenType::RIGHT_BRACKET: return "]";
        case TokenType::LEFT_BRACE: return "{";
        case TokenType::RIGHT_BRACE: return "}";
        case TokenType::LEFT_DOUBLE_BRACE: return "{{";
        case TokenType::RIGHT_DOUBLE_BRACE: return "}}";
        case TokenType::STRING_LITERAL: return "STRING_LITERAL";
        case TokenType::COMMA: return ",";
        case TokenType::COLON: return ":";
        case TokenType::EQUAL: return "=";
        case TokenType::DOUBLE_QUOTE: return "\"";
        default: return "UNKNOWN";
    }
}

std::ostream & operator<<(std::ostream &os, const Token &t)
{
    return os << t.text;
}

std::ostream & operator<<(std::ostream &os, const TokenRef &t)
{
    if(!t.ref) return os;
    return os << t->text;
}
}
