#pragma once

#include "SectionNode.hpp"

namespace usagi::negi
{
class ScriptNode : public ASTNode
{
    std::vector<SectionNode> mSections;

    void parseSection(ParsingContext *ctx);

public:
    void parse(ParsingContext *ctx) override;
    void check(SceneContext *ctx) override;
    void generate(SceneContext *ctx) override;
    void print(PrintContext &ctx) override;

    const std::vector<SectionNode> & sections() const
    {
        return mSections;
    }
};
}
