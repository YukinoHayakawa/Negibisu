#pragma once

#include <memory>

#include "LineNode.hpp"

namespace usagi::negibisu
{
class SectionNode : public ASTNode
{
	TokenRef mScriptName;
	TokenRef mDisplayName;

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
}
