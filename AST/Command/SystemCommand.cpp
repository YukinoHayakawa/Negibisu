#include "SystemCommand.hpp"

#include <Negibisu/Semantic/SceneContext.hpp>
#include <Negibisu/AST/PrintContext.hpp>

namespace usagi::negi
{
/*
 * SystemCommand
 */

void SystemCommand::check(SceneContext *ctx)
{
}

/*
 * SystemWaitInputCommand
 */

ParameterList SystemWaitInputCommand::parameterInfo() const
{
    NEGI_RETURN_PARAMS(
    );
}

void SystemWaitInputCommand::generate(SceneContext *ctx) const
{
    ctx->print("wait();");
}

void SystemWaitInputCommand::print(PrintContext &ctx) const
{
    ctx.print("SYSTEM_WAIT_INPUT");
}
}
