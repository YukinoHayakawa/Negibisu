#include "ASTNode.hpp"

#include <cassert>

#include <fmt/printf.h>

#include <Usagi/Core/Logging.hpp>

namespace usagi::negibisu
{
TokenType ASTNode::currentType() const
{
	if(mTokenBegin != mTokenEnd)
		return mTokenBegin->type;
	return TokenType::NEWLINE;
}

TokenType ASTNode::nextType(const std::size_t lookahead) const
{
	const auto next = mTokenBegin + lookahead;
	assert(next < mTokenEnd);
	return next->type;
}

const Token & ASTNode::cur() const
{
	assert(streamNotEnded());

	return *mTokenBegin;
}

void ASTNode::advance()
{
	assert(streamNotEnded());
	++mTokenBegin;
}

void ASTNode::skipNewLines()
{
	while(currentType() == TokenType::NEWLINE)
		advance();
}

bool ASTNode::streamEnded() const
{
	return mTokenBegin == mTokenEnd;
}

bool ASTNode::streamNotEnded() const
{
	return !streamEnded();
}

void ASTNode::expect(const TokenType token_type) const
{
	if(currentType() != token_type)
		error(fmt::format("Expected a {}", tokenSymbol(token_type)));
}

void ASTNode::consume(const TokenType token_type)
{
	expect(token_type);
	advance();
}

std::string_view ASTNode::consumeString()
{
	expect(TokenType::STRING_LITERAL);
	const auto ret = cur().text;
	advance();
	return ret;
}

void ASTNode::error(const std::string &msg) const
{
	fmt::print("Error at Line {}, Col {}: {}.\n",
		mTokenBegin->line,
		mTokenBegin->column,
		msg
	);
	throw std::runtime_error("Syntax error.");
}

ASTNode::ASTNode(
	TokenStreamIterator &token_begin,
	const TokenStreamIterator &token_end)
	: mTokenBegin(token_begin)
	, mTokenEnd(token_end)
{
}

void DialogNode::parse()
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

void DialogNode::print(std::string &indentation)
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
				error("Expected a , or }");
		}
	}
}

void CommandNode::parse()
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

void CommandNode::print(std::string &indentation)
{
	fmt::print("{}COMMAND: name={}, args=[{}]\n",
		indentation,
		mCommandName,
		fmt::join(mArgs.begin(), mArgs.end(), ", ")
	);
}

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
				error("Expected a line of text or a command");
		}
	}
}

void SectionNode::parseDialog()
{
	mLines.push_back(std::make_unique<DialogNode>(mTokenBegin, mTokenEnd));
	mLines.back()->parse();
}

void SectionNode::parseCommand()
{
	mLines.push_back(std::make_unique<CommandNode>(mTokenBegin, mTokenEnd));
	mLines.back()->parse();
}

void SectionNode::parse()
{
	parseTitle();
	parseContent();
}

void ScriptNode::parseSection()
{
	mSections.emplace_back(mTokenBegin, mTokenEnd);
	mSections.back().parse();
}

void ScriptNode::parse()
{
	while(streamNotEnded())
	{
		skipNewLines();
		parseSection();
	}
}

void ScriptNode::print(std::string &indentation)
{
	fmt::print("{}SCRIPT\n", indentation);
	indentation.append(INDENTATION, ' ');
	for(auto &&section : mSections)
		section.print(indentation);
	indentation.erase(indentation.end() - INDENTATION, indentation.end());
}
}
