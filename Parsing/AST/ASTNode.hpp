#pragma once

#include <vector>

#include "../../Lexical/Token.hpp"

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

	void advance();
	void skipNewLines();
	bool streamEnded() const;
	bool streamNotEnded() const;

	void expect(TokenType token_type) const;
	void consume(TokenType token_type);
	std::string_view consumeString();

	void error(const std::string &msg) const;

public:
	ASTNode(
		TokenStreamIterator &token_begin,
		const TokenStreamIterator &token_end);
	virtual ~ASTNode() = default;

	virtual void parse() = 0;
	virtual void print(std::string &indentation) = 0;
	// semantic validation
	virtual void validate() { }
};

class LineNode : public ASTNode
{
public:
	using ASTNode::ASTNode;
};

class DialogNode : public LineNode
{
	std::string_view mCharacter;
	std::string_view mAlias;
	std::string_view mExpression;
	std::string_view mPosition;
	std::string_view mText;
	bool mPause = false;

public:
	using LineNode::LineNode;

	void parse() override;
	void print(std::string &indentation) override;
};

class CommandNode : public LineNode
{
	std::string_view mCommandName;
	std::vector<std::string_view> mArgs;

	void parseArgs();

public:
	using LineNode::LineNode;

	void parse() override;
	void print(std::string &indentation) override;
};

class SectionNode : public ASTNode
{
	std::string_view mScriptName;
	std::string_view mDisplayName;

	std::vector<std::unique_ptr<LineNode>> mLines;

	void parseTitle();
	void parseContent();
	void parseDialog();
	void parseCommand();

public:
	using ASTNode::ASTNode;

	void parse() override;
	void print(std::string &indentation) override;
};

class ScriptNode : public ASTNode
{
	std::vector<SectionNode> mSections;

	void parseSection();

public:
	using ASTNode::ASTNode;

	void parse() override;
	void print(std::string &indentation) override;
};
}
