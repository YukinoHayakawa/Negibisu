#pragma once

#include <Negibisu/AST/Statement/IntrinsicCommand.hpp>

namespace usagi::negi
{
class SystemCommand : public IntrinsicCommand
{
};

class SystemWaitInputCommand : public SystemCommand
{
public:
    std::initializer_list<CommandParameterInfo> parameterInfo() const override;
    void fillArguments(
        SceneContext *ctx,
        const std::vector<TokenRef> &args) override;

    void check(SceneContext *ctx) override;
    void generate(SceneContext *ctx) override;
    void print(PrintContext &ctx) override;
};
}
