#include "DialogNode.hpp"

#include "../Semantic/SymbolTable.hpp"

namespace usagi::negibisu
{
void DialogNode::parse(SymbolTable *table)
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
            table->addStringLiteral(mAlias.ref);
            mCharacter = consumeString();
        }
        table->lookup(mCharacter.ref, SymbolType::CHARACTER);
        // optional expression and position change
        if(currentType() == TokenType::COMMA)
        {
            advance();
            mExpression = consumeString();
            table->lookup(mExpression.ref, SymbolType::EXPRESSION);
            consume(TokenType::COMMA);
            mPosition = consumeString();
            table->lookup(mPosition.ref, SymbolType::POSITION);
        }
        consume(TokenType::RIGHT_BRACKET);
    }
    mText = consumeString();
    table->addStringLiteral(mText.ref);
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
