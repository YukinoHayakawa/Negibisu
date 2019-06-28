#pragma once

#include "SectionNode.hpp"

namespace usagi::negi
{
class ScriptNode : public ASTNode
{
    std::vector<SectionNode> mSections;

    void parseSection();

public:
    using ASTNode::ASTNode;

    void parse(SymbolTable *table = nullptr) override;
    void print(std::string &indentation) override;

    const std::vector<SectionNode> & sections() const
    {
        return mSections;
    }
};
}
