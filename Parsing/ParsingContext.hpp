#pragma once

#include <vector>
#include <list>

#include <Negibisu/Lexical/Token.hpp>

namespace usagi::negi
{
struct ParsingContext
{
    const std::vector<Token> &tokens;
    // modifying token vector will invalidate the pointers, therefore put
    // extra tokens in a list.
    std::list<Token> extra_tokens;

    using TokenStreamIterator = std::vector<Token>::const_iterator;

    TokenStreamIterator cursor;
    const TokenStreamIterator end;

    explicit ParsingContext(const std::vector<Token> &tokens);

    TokenRef createToken(TokenType type, std::string_view text);
};
}
