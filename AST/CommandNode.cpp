#include "CommandNode.hpp"

namespace usagi::negibisu
{
void CommandNode::parseArgs()
{
	while(streamNotEnded())
	{
		mArgs.push_back(consumeString());
		switch(currentType())
		{
		case TokenType::COMMA:
			advance();
			continue;

		case TokenType::RIGHT_BRACE:
			return;

		default:
			syntaxError(
                "Expected a {} or {}.",
                tokenSymbol(TokenType::COMMA),
                tokenSymbol(TokenType::RIGHT_BRACE)
            );
		}
	}
}

void CommandNode::parse(SymbolTable *table)
{
	consume(TokenType::LEFT_BRACE);
	mCommandName = consumeString();
	if(currentType() == TokenType::COLON)
	{
		advance();
		parseArgs();
	}
	consume(TokenType::RIGHT_BRACE);
}

void CommandNode::print(std::string& indentation)
{
	fmt::print("{}COMMAND: name=\"{}\", args=[{}]\n",
		indentation,
		mCommandName,
		fmt::join(mArgs.begin(), mArgs.end(), ", ")
	);
}
}
