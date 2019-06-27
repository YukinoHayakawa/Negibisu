#include "SectionNode.hpp"

#include "CommandNode.hpp"
#include "DialogNode.hpp"

namespace usagi::negibisu
{
void SectionNode::print(std::string &indentation)
{
	fmt::print(
		"{}SECTION: script_name=\"{}\", display_name=\"{}\"\n",
		indentation,
		mScriptName, mDisplayName
	);
	indentation.append(INDENTATION, ' ');
	for(auto &&line : mLines)
		line->print(indentation);
	indentation.erase(indentation.end() - INDENTATION, indentation.end());
}

void SectionNode::parseTitle()
{
	consume(TokenType::SHARP);
	mScriptName = consumeString();
	consume(TokenType::COLON);
	mDisplayName = consumeString();

    mSymbolTable.lookup(mScriptName.ref, SymbolType::SCRIPT);
}

void SectionNode::parseContent()
{
	while(streamNotEnded())
	{
		switch(currentType())
		{
			case TokenType::LEFT_BRACKET:
			case TokenType::STRING_LITERAL:
				parseDialog();
				continue;
			case TokenType::LEFT_BRACE:
				parseCommand();
				continue;
			// next section
			case TokenType::SHARP:
				return;
			case TokenType::NEWLINE:
				advance();
				continue;
			default:
				syntaxError("Expected a line of text or a command.");
		}
	}
}

void SectionNode::parseDialog()
{
	mLines.push_back(std::make_unique<DialogNode>(mTokenBegin, mTokenEnd));
	mLines.back()->parse(&mSymbolTable);
}

void SectionNode::parseCommand()
{
	mLines.push_back(std::make_unique<CommandNode>(mTokenBegin, mTokenEnd));
	mLines.back()->parse(&mSymbolTable);
}

void SectionNode::parse(SymbolTable *table)
{
	parseTitle();
	parseContent();
}
}
