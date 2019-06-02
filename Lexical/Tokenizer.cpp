﻿#include "Tokenizer.hpp"

#include <fmt/printf.h>

namespace usagi::negibisu
{
void Tokenizer::tokenize()
{
	while(const auto this_char = cur())
	{
		skipWhiteSpace();
		beginToken();
		if(this_char == '#')
		{
			advance();
			if(const auto last_type = lastTokenType();
				last_type == TokenType::NEWLINE
				|| last_type == TokenType::SHARP)
			{
				endToken(TokenType::SHARP);
				mEnvironment = Environment::TITLE;
			}
			continue;
		}
		if(this_char == '{')
		{
			if(peek() == '{')
			{
				advance();
				advance();
				endToken(TokenType::LEFT_DOUBLE_BRACE);
				mEnvironment = Environment::COMMENT;
			}
			else
			{
				advance();
				endToken(TokenType::LEFT_BRACE);
				mEnvironment = Environment::COMMAND;
			}
			continue;
		}
		if(this_char == '}')
		{
			if(peek() == '}')
			{
				advance();
				advance();
				endToken(TokenType::RIGHT_DOUBLE_BRACE);
				mEnvironment = Environment::GLOBAL;
			}
			else
			{
				advance();
				endToken(TokenType::RIGHT_BRACE);
				mEnvironment = Environment::GLOBAL;
			}
			continue;
		}
		if(this_char == '[')
		{
			advance();
			endToken(TokenType::LEFT_BRACKET);
			mEnvironment = Environment::COMMAND;
			continue;
		}
		if(this_char == ']')
		{
			advance();
			endToken(TokenType::RIGHT_BRACKET);
			mEnvironment = Environment::GLOBAL;
			continue;
		}
		if(mEnvironment == Environment::COMMAND)
		{
			if(this_char == ':')
			{
				advance();
				endToken(TokenType::COLON);
				continue;
			}
			if(this_char == '=')
			{
				advance();
				endToken(TokenType::EQUAL);
				continue;
			}
			if(this_char == ',')
			{
				advance();
				endToken(TokenType::COMMA);
				continue;
			}
		}
		if(mEnvironment == Environment::TITLE)
		{
			if(this_char == ':')
			{
				advance();
				endToken(TokenType::COLON);
				continue;
			}
		}
		readStringLiteral();
	}
}

void Tokenizer::onNewLine()
{
	// continue last line
	if(last() == '\\')
		return;

	// append newline token
	beginToken();
	endToken(TokenType::NEWLINE);

	// reset environment
	mEnvironment = Environment::GLOBAL;
}

void Tokenizer::beginToken()
{
	mTempToken.line = currentLine();
	mTempToken.column = currentColumn();
	beginSubstring();
}

void Tokenizer::endToken(TokenType type, std::size_t trim_back)
{
	mTempToken.type = type;
	mTempToken.text = endSubstring(trim_back);
	mTokens.push_back(mTempToken);
}

bool Tokenizer::isTokenChar(char32_t c)
{
	return isCommandTokenChar(c)
		// these are only effective within [] and {} pairs
		|| c == ','
		|| c == '='
		|| c == ':';
}

bool Tokenizer::isCommandTokenChar(char32_t c)
{
	return c == '['
		|| c == ']'
		|| c == '{'
		|| c == '}';
}

void Tokenizer::readStringLiteral()
{
	skipWhiteSpace();
	beginToken();
	const std::size_t begin_u8_size = currentUtf8Size();
	std::size_t last_non_space_size = begin_u8_size;
	bool escape_next = false;
	// if not in command block, read til next command begin token,
	// otherwise read til next control token
	while(const auto next_char = cur())
	{
		// line not continued, break.
		if(next_char == '\n' && last() != '\\')
		{
			break;
		}
		// escape next character
		if(next_char == '\\')
		{
			advance(false);
			escape_next = true;
			goto goto_next;
		}
		// always append the escaped character
		if(escape_next)
		{
			advance();
			escape_next = false;
			goto goto_next;
		}
		if(mEnvironment == Environment::GLOBAL)
		{
			if(isCommandTokenChar(next_char))
				break;
		}
		else if(mEnvironment == Environment::COMMAND)
		{
			if(isTokenChar(next_char))
				break;
		}
		else if(mEnvironment == Environment::COMMENT)
		{
			if(next_char == '}' && peek() == '}')
				break;
		}
		else if(mEnvironment == Environment::TITLE)
		{
			if(next_char == ':')
				break;
		}
		// move onto next char
		advance();

goto_next:
		if(!isSpaceChar(next_char))
			last_non_space_size = currentUtf8Size();
	}
	const auto trim_back_space = currentUtf8Size() - last_non_space_size;
	const auto len = currentUtf8Size() - begin_u8_size - trim_back_space;
	if(len > 0)
	{
		endToken(TokenType::STRING_LITERAL, trim_back_space);
	}
}

TokenType Tokenizer::lastTokenType() const
{
	if(mTokens.empty())
		return TokenType::NEWLINE;
	return mTokens.back().type;
}

void Tokenizer::dumpTokens()
{
	for(auto &&token : mTokens)
	{
		fmt::print("Line {}, Col {}: [{}] {}\n",
			token.line, token.column,
			tokenName(token.type),
			token.text
		);
	}
}
}
