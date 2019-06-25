#include "ASTNode.hpp"

#include <cassert>

#include <fmt/printf.h>

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

TokenRef ASTNode::curRef() const
{
	return { &*mTokenBegin };
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

TokenRef ASTNode::consumeString()
{
	expect(TokenType::STRING_LITERAL);
	const auto ret = curRef();
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
}
