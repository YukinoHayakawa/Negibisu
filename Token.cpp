#include "Token.hpp"

const char * tokenName(TokenType token)
{
    switch(token)
    {
        case TokenType::STRING_LITERAL: return "STRING_LITERAL";
        case TokenType::LEFT_BRACKET: return "LEFT_BRACKET";
        case TokenType::RIGHT_BRACKET: return "RIGHT_BRACKET";
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::LEFT_DOUBLE_BRACE: return "LEFT_DOUBLE_BRACE";
        case TokenType::RIGHT_DOUBLE_BRACE: return "RIGHT_DOUBLE_BRACE";
        case TokenType::COMMA: return "COMMA";
        case TokenType::COLON: return "COLON";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::NEWLINE: return "NEWLINE";
        case TokenType::END_OF_TOKENS: return "END_OF_TOKENS";
        default: return "UNKNOWN";
    }
}
