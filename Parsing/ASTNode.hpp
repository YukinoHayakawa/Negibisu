#pragma once

#include <vector>

#include <fmt/ostream.h>

#include "../Lexical/Token.hpp"

namespace usagi::negibisu
{
class ASTNode
{
protected:
	static constexpr int INDENTATION = 2;

	using TokenStreamIterator = std::vector<Token>::const_iterator;

	TokenStreamIterator &mTokenBegin;
	const TokenStreamIterator &mTokenEnd;

	TokenType currentType() const;
	TokenType nextType(std::size_t lookahead = 1) const;
	const Token & cur() const;
	TokenRef curRef() const;

	void advance();
	void skipNewLines();
	bool streamEnded() const;
	bool streamNotEnded() const;

	void expect(TokenType token_type) const;
	void consume(TokenType token_type);
	TokenRef consumeString();

	void error(const std::string &msg) const;

public:
	ASTNode(
		TokenStreamIterator &token_begin,
		const TokenStreamIterator &token_end);
	virtual ~ASTNode() = default;

	virtual void parse() = 0;
	virtual void print(std::string &indentation) = 0;
	// semantic checking
	// virtual void check() = 0;
};
}
