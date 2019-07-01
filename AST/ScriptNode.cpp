#include "ScriptNode.hpp"

#include <Negibisu/Parsing/ParsingContext.hpp>

#include "PrintContext.hpp"

namespace usagi::negi
{
void ScriptNode::parseSection(ParsingContext *ctx)
{
    mSections.emplace_back();
    mSections.back().parse(ctx);
}

void ScriptNode::parse(ParsingContext *ctx)
{
    while(ctx->streamNotEnded())
    {
        ctx->skipNewLines();
        parseSection(ctx);
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

void ScriptNode::print(PrintContext &ctx)
{
    ctx.print("SCRIPT");
    ctx.push();
    for(auto&& section : mSections)
        section.print(ctx);
    ctx.pop();
}
}
