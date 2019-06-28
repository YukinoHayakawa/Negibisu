#pragma once

#include <memory>

#include "LineNode.hpp"
#include "../Semantic/SymbolTable.hpp"

namespace usagi::negibisu
{
class SectionNode : public ASTNode
{
    SymbolTable mSymbolTable;
    TokenRef mScriptName;
    TokenRef mDisplayName;

    std::vector<std::unique_ptr<LineNode>> mLines;

    void parseTitle();
    void parseContent();
    void parseDialog();
    void parseCommand();

public:
    using ASTNode::ASTNode;

    void parse(SymbolTable *table) override;
    void print(std::string &indentation) override;

    const SymbolTable & symbolTable() const
    {
        return mSymbolTable;
    }

    std::string_view scriptName() const
    {
        return mScriptName->text;
    }
};
}
