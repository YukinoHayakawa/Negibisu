#include "SystemCommand.hpp"

#include <Negibisu/Semantic/SceneContext.hpp>
#include <Negibisu/AST/PrintContext.hpp>

namespace usagi::negi
{
std::initializer_list<CommandParameterInfo>
SystemWaitInputCommand::parameterInfo() const
{
    static std::initializer_list<CommandParameterInfo> params = {
    };
    return params;
}

void SystemWaitInputCommand::check(SceneContext *ctx)
{
}

void SystemWaitInputCommand::generate(SceneContext *ctx) const
{
    ctx->print("_w();");
}

void SystemWaitInputCommand::print(PrintContext &ctx) const
{
    ctx.print("SYSTEM_WAIT_INPUT");
}

void SystemWaitInputCommand::fillArguments(
    SceneContext *ctx,
    const std::vector<TokenRef> &args)
{
    IntrinsicCommand::fillArguments(ctx, args);
}
}
