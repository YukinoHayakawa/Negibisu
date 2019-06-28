#pragma once

#include "LineNode.hpp"

namespace usagi::negibisu
{
class DialogNode : public LineNode
{
    TokenRef mCharacter;
    TokenRef mAlias;
    TokenRef mExpression;
    TokenRef mPosition;
    TokenRef mText;
    bool mPause = false;

public:
    using LineNode::LineNode;

    void parse(SymbolTable *table) override;
    void print(std::string &indentation) override;
};;
}
