#pragma once

#include "LineNode.hpp"

namespace usagi::negi
{
class CommandNode : public LineNode
{
    TokenRef mCommandName;
    // todo do not use vector for memory efficiency
    std::vector<TokenRef> mArgs;

    void parseArgs();

public:
    using LineNode::LineNode;

    void parse(SymbolTable *table) override;
    void print(std::string &indentation) override;
};
}
