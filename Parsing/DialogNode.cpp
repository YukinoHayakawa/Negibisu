#include "DialogNode.hpp"

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
			mCharacter = consumeString();
		}
		// optional expression and position change
		if(currentType() == TokenType::COMMA)
		{
			advance();
			mExpression = consumeString();
			consume(TokenType::COMMA);
			mPosition = consumeString();
		}
		consume(TokenType::RIGHT_BRACKET);
	}
	mText = consumeString();
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
