#include "IntrinsicCommand.hpp"

#include <unordered_map>
#include <functional>

#include <Negibisu/AST/Command/CharacterCommand.hpp>
#include <Negibisu/AST/Command/ImageCommand.hpp>
#include <Negibisu/Semantic/SceneContext.hpp>

namespace usagi::negi
{
namespace
{
#define NEGI_DEFINE_BUILTIN(name, type) { \
    name, \
    []() { \
        return std::make_unique<type>(); \
    } \
} \
/**/

const std::unordered_map<
    std::string_view,
    std::function<std::unique_ptr<IntrinsicCommand>()>
> BUILTIN_COMMANDS = {
    NEGI_DEFINE_BUILTIN("changeImage", ImageChangeCommand),
    NEGI_DEFINE_BUILTIN("clearImage", ImageClearCommand),
    NEGI_DEFINE_BUILTIN("enter", CharacterEnterStageCommand),
    NEGI_DEFINE_BUILTIN("exit", CharacterExitStageCommand),
    NEGI_DEFINE_BUILTIN("move", CharacterMoveCommand),
    NEGI_DEFINE_BUILTIN("expr", CharacterChangeExpressionCommand),
};
#undef NEGI_DEFINE_BUILTIN
}

void IntrinsicCommand::fillArguments(
    SceneContext *ctx,
    const std::vector<TokenRef> &args)
{
    const auto param_info = parameterInfo();
    assert(args.size() == param_info.size());

    // check argument types
    for(std::size_t i = 0; i < param_info.size(); ++i)
    {
        switch((param_info.begin() + i)->type)
        {
            case CommandParameterType::STRING:
                break;
            case CommandParameterType::INT:
                if(!args[i]->convertibleTo<int>())
                {
                    ctx->semanticError(args[i], "Expected an Int.");
                }
                break;
            case CommandParameterType::FLOAT:
                if(!args[i]->convertibleTo<float>())
                {
                    ctx->semanticError(args[i], "Expected an Float.");
                }
                break;
            default:
                throw std::logic_error("Unexpected command parameter type.");
        }
    }
}

void IntrinsicCommand::parse(ParsingContext *ctx)
{
    throw std::logic_error(
        "Intrinsic command must be constructed by explicitly providing "
        "arguments.");
}

std::unique_ptr<IntrinsicCommand> IntrinsicCommand::find(std::string_view name)
{
    const auto gen = BUILTIN_COMMANDS.find(name);
    std::unique_ptr<IntrinsicCommand> cmd;
    if(gen != BUILTIN_COMMANDS.end())
    {
        cmd = gen->second();
    }
    return std::move(cmd);
}
}
