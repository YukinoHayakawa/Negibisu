#include "ASTNode.hpp"

#include <cassert>

#include <fmt/printf.h>

namespace usagi::negi
{
TokenType ASTNode::currentType() const
{
    if(mParsingContext->cursor != mParsingContext->end)
        return mParsingContext->cursor->type;
    return TokenType::END_OF_STREAM;
}

TokenType ASTNode::nextType(const std::size_t lookahead) const
{
    const auto next = mParsingContext->cursor + lookahead;
    assert(next < mParsingContext->end);
    return next->type;
}

const Token & ASTNode::cur() const
{
    assert(streamNotEnded());

    return *mParsingContext->cursor;
}

TokenRef ASTNode::curRef() const
{
    return { &*mParsingContext->cursor };
}

void ASTNode::advance()
{
    assert(streamNotEnded());
    ++mParsingContext->cursor;
}

void ASTNode::skipNewLines()
{
    while(currentType() == TokenType::NEWLINE)
        advance();
}

bool ASTNode::streamEnded() const
{
    return mParsingContext->cursor == mParsingContext->end;
}

bool ASTNode::streamNotEnded() const
{
    return !streamEnded();
}

void ASTNode::expect(const TokenType token_type) const
{
    if(currentType() != token_type)
        syntaxError("Expected a {}.", tokenSymbol(token_type));
}

void ASTNode::consume(const TokenType token_type)
{
    expect(token_type);
    advance();
}

TokenRef ASTNode::consumeString()
{
    expect(TokenType::STRING_LITERAL);
    const auto ret = curRef();
    advance();
    return ret;
}

void ASTNode::proceedToNextLine()
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

ASTNode::ASTNode(ParsingContext *context)
    : mParsingContext(context)
{
}
}
