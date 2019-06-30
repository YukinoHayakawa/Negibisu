#include "ParsingContext.hpp"

namespace usagi::negi
{
ParsingContext::ParsingContext(const std::vector<Token> &tokens)
    : tokens(tokens)
    , cursor(tokens.begin())
    , end(tokens.end())
{
}

TokenRef ParsingContext::createToken(
    TokenType type,
    std::string_view text)
{
    extra_tokens.push_back({
        .index = tokens.size(),
        .type = type,
        .text = text
    });
    return { .ref = &extra_tokens.back() };
}
}
