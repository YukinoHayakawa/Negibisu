#include "ScriptNode.hpp"

namespace usagi::negi
{
void ScriptNode::parseSection()
{
    mSections.emplace_back(mParsingContext);
    mSections.back().parse(nullptr);
}

void ScriptNode::parse(SceneContext *ctx)
{
    while(streamNotEnded())
    {
        skipNewLines();
        parseSection();
    }
}

void ScriptNode::check(SceneContext *ctx)
{
    for(auto &&s : mSections)
        s.check(nullptr);
}

void ScriptNode::generate(SceneContext *ctx)
{
    for(auto &&s : mSections)
        s.generate(nullptr);
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
