#include "ParsingContext.hpp"

namespace usagi::negi
{
ParsingContext::ParsingContext(const std::vector<Token> &tokens)
    : tokens(tokens)
    , cursor(tokens.begin())
    , end(tokens.end())
{
}

TokenRef ParsingContext::createTokenFromStringView(
    TokenType type,
    std::string_view text)
{
    extra_tokens.push_back({
        /*.index = */ tokens.size(),
        /* .pos */ { },
        /*.type = */ type,
        /*.text = */ text
    });
    return { /*.ref =*/ &extra_tokens.back() };
}

TokenRef ParsingContext::createTokenFromString(TokenType type, std::string text)
{
    extra_strings.push_back(std::move(text));
    return createTokenFromStringView(type,
        std::string_view(extra_strings.back()));
}

TokenType ParsingContext::currentType() const
{
    if(cursor != end)
        return cursor->type;
    return TokenType::END_OF_STREAM;
}

TokenType ParsingContext::peekType(const std::size_t lookahead) const
{
    const auto next = cursor + lookahead;
    return next < end ? next->type : TokenType::END_OF_STREAM;
}

const Token * ParsingContext::peek(std::size_t lookahead) const
{
    const auto next = cursor + lookahead;
    return next < end ? &*next : nullptr;
}

const Token &ParsingContext::cur() const
{
    assert(streamNotEnded());

    return *cursor;
}

TokenRef ParsingContext::curRef() const
{
    return { &*cursor };
}

void ParsingContext::advance()
{
    assert(streamNotEnded());
    ++cursor;
}

void ParsingContext::skipNewLines()
{
    while(currentType() == TokenType::NEWLINE)
        advance();
}

bool ParsingContext::streamEnded() const
{
    return cursor == end;
}

bool ParsingContext::streamNotEnded() const
{
    return !streamEnded();
}

void ParsingContext::expect(const TokenType token_type) const
{
    if(currentType() != token_type)
        syntaxError("Expected a {}.", tokenSymbol(token_type));
}

void ParsingContext::consume(const TokenType token_type)
{
    expect(token_type);
    advance();
}

TokenRef ParsingContext::consumeString()
{
    expect(TokenType::STRING_LITERAL);
    const auto ret = curRef();
    advance();
    return ret;
}

void ParsingContext::proceedToNextLine()
{
    while(streamNotEnded())
    {
        switch(currentType())
        {
            case TokenType::NEWLINE:
                return;

            default:
                advance();
        }
    }
}
}
