﻿#pragma once

#include <memory>

#include <Negibisu/Semantic/SceneContext.hpp>

#include "StatementNode.hpp"

namespace usagi::negi
{
class SectionNode : public ASTNode
{
    SceneContext mSceneContext;
    TokenRef mScriptName;
    TokenRef mDisplayName;

    std::vector<std::unique_ptr<StatementNode>> mStatements;

    void parseTitle(ParsingContext *ctx);
    void parseContent(ParsingContext *ctx);
    void parseLine(ParsingContext *ctx);

public:
    void parse(ParsingContext *ctx) override;
    void check(SceneContext *ctx) override;
    void generate(SceneContext *ctx) override;

    void print(PrintContext &ctx) override;

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
