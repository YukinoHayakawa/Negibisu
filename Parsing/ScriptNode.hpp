#pragma once

#include "SectionNode.hpp"

namespace usagi::negibisu
{
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
