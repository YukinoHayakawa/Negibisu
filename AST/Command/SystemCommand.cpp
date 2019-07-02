#include "SystemCommand.hpp"

#include <Negibisu/Semantic/SceneContext.hpp>
#include <Negibisu/AST/PrintContext.hpp>

namespace usagi::negi
{
std::initializer_list<CommandParameterInfo>
SystemWaitInputCommand::parameterInfo() const
{
    return { };
}

void SystemWaitInputCommand::check(SceneContext *ctx)
{
}

void SystemWaitInputCommand::generate(SceneContext *ctx)
{
    ctx->print("_w();");
}

void SystemWaitInputCommand::print(PrintContext &ctx)
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
