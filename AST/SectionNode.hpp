#pragma once

#include <memory>

#include <Negibisu/Semantic/SceneContext.hpp>

#include "LineNode.hpp"

namespace usagi::negi
{
class SectionNode : public ASTNode
{
    SceneContext mSceneContext;
    TokenRef mScriptName;
    TokenRef mDisplayName;

    std::vector<std::unique_ptr<LineNode>> mLines;

    void parseTitle();
    void parseContent();
    void parseDialog();
    void parseCommand();

public:
    using ASTNode::ASTNode;

    void parse(SceneContext *ctx) override;
    void print(std::string &indentation) override;

    const SceneContext & context() const
    {
        return mSceneContext;
    }

    std::string_view scriptName() const
    {
        return mScriptName->text;
    }
};
}
