#pragma once

#include <vector>

#include "../StringParser.hpp"
#include "Token.hpp"

namespace usagi::negibisu
{
class Tokenizer : public StringParser
{
protected:
	std::vector<Token> mTokens;

	bool mPlainText = true;
	bool mComment = false;
	Token mTempToken;

	enum class Environment
	{
		GLOBAL,
		COMMAND,
		COMMENT,
		TITLE,
	} mEnvironment = Environment::GLOBAL;

	void beginToken();
	void endToken(TokenType type, std::size_t trim_back = 0);

	static bool isTokenChar(char32_t c);
	static bool isCommandTokenChar(char32_t c);
	void readStringLiteral();
	TokenType lastTokenType() const;

	void onNewLine() override;

public:
	using StringParser::StringParser;

	void tokenize();
	void dumpTokens();
	const std::vector<Token> & tokens() const { return mTokens; }
};
}
