#include "Tokenizer.hpp"

#include <fmt/printf.h>

namespace usagi::negibisu
{
void Tokenizer::tokenize()
{
	while(cur())
	{
		skipWhiteSpace();
		beginToken();
		const auto this_char = cur();
		// todo: preprocessor directives
		// if(this_char == '$' && lastTokenType() == TokenType::NEWLINE)
		// {
		//
		// }
		if(this_char == '#')
		{
			if(const auto last_type = lastTokenType();
				last_type == TokenType::NEWLINE
				|| last_type == TokenType::SHARP)
			{
				advance();
				endToken(TokenType::SHARP);
				enterEnvironment(Environment::TITLE);
				continue;
			}
		}
		if(this_char == '{')
		{
			if(next() == '{')
			{
				ignoreComment();
			}
			else
			{
				advance();
				endToken(TokenType::LEFT_BRACE);
				enterEnvironment(Environment::COMMAND);
			}
			continue;
		}
		if(this_char == '}')
		{
			if(next() == '}')
			{
				// }} should already be consumed by ignoreComment()
				assert(false);
			}
			else
			{
				exitEnvironment(Environment::COMMAND);
				advance();
				endToken(TokenType::RIGHT_BRACE);
			}
			continue;
		}
		if(this_char == '[')
		{
			advance();
			endToken(TokenType::LEFT_BRACKET);
			enterEnvironment(Environment::CHARACTER);
			continue;
		}
		if(this_char == ']')
		{
			exitEnvironment(Environment::CHARACTER);
			advance();
			endToken(TokenType::RIGHT_BRACKET);
			continue;
		}
		if(currentEnvironment() == Environment::COMMAND
			|| currentEnvironment() == Environment::CHARACTER)
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
		if(currentEnvironment() == Environment::TITLE)
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

void Tokenizer::finalize()
{
	beginToken();
	endToken(TokenType::END_OF_STREAM);
}

void Tokenizer::resetEnvironment()
{
	mEnv = { Environment::GLOBAL };
}

void Tokenizer::onNewLine()
{
	// append newline token
	beginToken();
	endToken(TokenType::NEWLINE);

	if(const auto cur_env = currentEnvironment();
		cur_env != Environment::GLOBAL)
	{
		assert(mEnv.size() > 1);

		switch(cur_env)
		{
			case Environment::COMMAND:
				error("Expected a }.");
				break;
			case Environment::CHARACTER:
				error("Expected a ].");
				break;
			case Environment::COMMENT:
				error("Expected a }}.");
				break;
			default: ;
		}
		// reset env on newline
		resetEnvironment();
	}
}

Tokenizer::Environment Tokenizer::currentEnvironment() const
{
	return mEnv.back();
}

void Tokenizer::enterEnvironment(Environment env)
{
	mEnv.push_back(env);
}

void Tokenizer::exitEnvironment(Environment env)
{
	assert(env != Environment::GLOBAL);
	const auto matched = env == currentEnvironment();

	if(matched)
	{
		assert(mEnv.size() > 1);
		mEnv.pop_back();
	}
	else
	{
		switch(env)
		{
			case Environment::COMMAND:
				error("Expected a matching {.");
				break;
			case Environment::CHARACTER:
				error("Expected a matching [.");
				break;
			case Environment::COMMENT:
				error("Expected a matching {{.");
				break;
			default:
				break;
		}
	}
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

bool Tokenizer::isOperatorCharInCharacterName(char32_t c)
{
	return isEnvironmentBoundaryChar(c)
		|| c == ','
		|| c == '='
		|| c == ':';
}

bool Tokenizer::isOperatorCharInCommand(char32_t c)
{
	return isEnvironmentBoundaryChar(c)
		|| c == ','
		|| c == ':';
}

bool Tokenizer::isOperatorCharInTitle(char32_t c)
{
	return isEnvironmentBoundaryChar(c)
		|| c == ':';
}

bool Tokenizer::isEnvironmentBoundaryChar(char32_t c)
{
	return c == '['
		|| c == ']'
		|| c == '{'
		|| c == '}';
}

void Tokenizer::readStringLiteral()
{
	// we executed skipWhiteSpace() before, so \n can only be at least the
	// second character
	assert(!isSpaceChar(cur()));

	const std::size_t begin_u8_size = currentUtf8Size();
	std::size_t last_non_space_size = begin_u8_size;
	bool escape_next = false;

	// if not in command block, read til next command begin token,
	// otherwise read til next control token
	while(const auto this_char = cur())
	{
		if(this_char == '\n')
		{
			break;
		}
		// escape next character. note that \\\n is hidden from by advance()
		if(this_char == '\\' && !escape_next)
		{
			advance(false);
			escape_next = true;
			goto read_next;
		}
		// always append the escaped character
		if(escape_next)
		{
			if(isSpaceChar(this_char))
			{
				skipWhiteSpace();
			}
			else
			{
				advance();
			}
			escape_next = false;
			goto read_next;
		}
		if(cur() == '{' && next() == '{')
		{
			ignoreComment();
			continue;
		}
		if(currentEnvironment() == Environment::GLOBAL)
		{
			if(isEnvironmentBoundaryChar(this_char))
				break;
		}
		else if(currentEnvironment() == Environment::COMMAND)
		{
			if(isOperatorCharInCommand(this_char))
				break;
		}
		else if(currentEnvironment() == Environment::CHARACTER)
		{
			if(isOperatorCharInCharacterName(this_char))
				break;
		}
		else if(currentEnvironment() == Environment::TITLE)
		{
			if(isOperatorCharInTitle(this_char))
				break;
		}
		// move onto next char
		advance();

read_next:
		if(!isSpaceChar(this_char))
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

void Tokenizer::ignoreComment()
{
	assert(cur() == '{' && next() == '{');
	advance(false);
	advance(false);
	enterEnvironment(Environment::COMMENT);
	while(cur())
	{
		if(cur() == '}' && next() == '}')
		{
			exitEnvironment(Environment::COMMENT);
			advance(false);
			advance(false);
			return;
		}
		advance(false, false, true);
	}
	error("Expected a }}.");
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
