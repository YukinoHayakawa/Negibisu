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
		CHARACTER,
		COMMENT,
		TITLE,
	};

	std::vector<Environment> mEnv = { Environment::GLOBAL };

	Environment currentEnvironment() const;
	void enterEnvironment(Environment env);
	void exitEnvironment(Environment env);

	void beginToken();
	void endToken(TokenType type, std::size_t trim_back = 0);

	static bool isEnvironmentBoundaryChar(char32_t c);
	static bool isOperatorCharInCharacterName(char32_t c);
	static bool isOperatorCharInCommand(char32_t c);
	static bool isOperatorCharInTitle(char32_t c);
	void readStringLiteral();
	TokenType lastTokenType() const;
	void ignoreComment();

	void onNewLine() override;
	void resetEnvironment();

public:
	using StringParser::StringParser;

	void tokenize();
	void finalize();
	void dumpTokens();
	const std::vector<Token> & tokens() const { return mTokens; }
};
}
