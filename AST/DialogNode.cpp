#include "DialogNode.hpp"

#include <Negibisu/Semantic/SymbolTable.hpp>
#include <Negibisu/Semantic/SceneContext.hpp>

namespace usagi::negi
{
void DialogNode::parse(SceneContext *ctx)
{
    if(currentType() == TokenType::LEFT_BRACKET)
    {
        advance();
        mCharacter = consumeString();
        // optional alias name
        if(currentType() == TokenType::EQUAL)
        {
            advance();
            mAlias = mCharacter;
            ctx->symbol_table.addStringLiteral(mAlias.ref);
            mCharacter = consumeString();
        }
        ctx->symbol_table.lookup(mCharacter.ref, SymbolType::CHARACTER);
        // optional expression and position change
        if(currentType() == TokenType::COMMA)
        {
            advance();
            mExpression = consumeString();
            ctx->symbol_table.lookup(mExpression.ref, SymbolType::EXPRESSION);
            consume(TokenType::COMMA);
            mPosition = consumeString();
            ctx->symbol_table.lookup(mPosition.ref, SymbolType::POSITION);
        }
        consume(TokenType::RIGHT_BRACKET);
    }
    // bug: text interleaved by commands does not show to be the same character
    mText = consumeString();
    ctx->symbol_table.addStringLiteral(mText.ref);
    if(currentType() == TokenType::NEWLINE)
    {
        mPause = true;
        advance();
    }
}

void DialogNode::print(std::string& indentation)
{
    fmt::print(
        "{}DIALOG: char=\"{}\", alias=\"{}\", "
        "expr=\"{}\", pos=\"{}\", text=\"{}\"\n",
        indentation,
        mCharacter,
        mAlias,
        mExpression,
        mPosition,
        mText
    );
}
}
