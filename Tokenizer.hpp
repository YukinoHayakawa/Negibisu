#pragma once

#include <string>
#include <vector>

#include <Usagi/Utility/Noncopyable.hpp>

#include "Token.hpp"

namespace usagi::negibisu
{
class Tokenizer : Noncopyable
{
	std::u32string mSource;
	std::string mU8Source;
	std::vector<Token> mTokens;
	int mCurrentLine = 1;
	int mCurrentColumn = 1;
	std::size_t mCurrentPos = 0;
	bool mPlainText = true;
	bool mComment = false;
	Token mTempToken;
	std::size_t mTempPos = 0;

	void beginToken();
	void endToken(TokenType token, std::size_t length);
	const char32_t & cur() const;
	const char32_t & advance();
	char32_t peek() const;
	static bool isTokenChar(char32_t c);
	static bool isCommandTokenChar(char32_t c);
	static bool isSpaceChar(char32_t c);
	void skipWhiteSpace();
	void readStringLiteral();
	void error();

public:
	void tokenize(std::istream &in);
	void dumpTokens();
	const std::vector<Token> & tokens() const { return mTokens; }
};
}
