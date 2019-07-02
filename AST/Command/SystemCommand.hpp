#pragma once

#include <Negibisu/AST/Statement/IntrinsicCommand.hpp>

namespace usagi::negi
{
class SystemCommand : public IntrinsicCommand
{
public:
    void check(SceneContext *ctx) override;
};

class SystemWaitInputCommand : public SystemCommand
{
public:
    ParameterList parameterInfo() const override;

    void generate(SceneContext *ctx) const override;
    void print(PrintContext &ctx) const override;
};
}
