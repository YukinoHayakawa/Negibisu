#include "ScriptNode.hpp"

namespace usagi::negibisu
{
void ScriptNode::parseSection()
{
	mSections.emplace_back(mTokenBegin, mTokenEnd);
	mSections.back().parse();
}

void ScriptNode::parse()
{
	while(streamNotEnded())
	{
		skipNewLines();
		parseSection();
	}
}

void ScriptNode::print(std::string& indentation)
{
	fmt::print("{}SCRIPT\n", indentation);
	indentation.append(INDENTATION, ' ');
	for(auto&& section : mSections)
		section.print(indentation);
	indentation.erase(indentation.end() - INDENTATION, indentation.end());
}
}
