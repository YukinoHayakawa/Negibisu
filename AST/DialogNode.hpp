#pragma once

#include "LineNode.hpp"

namespace usagi::negi
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

    void parse(SceneContext *ctx) override;
    void check(SceneContext *ctx) override;
    void generate(SceneContext *ctx) override;

    void print(std::string &indentation) override;
};;
}
