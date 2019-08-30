#include "IntrinsicCommand.hpp"

#include <unordered_map>
#include <functional>

#include <Negibisu/Semantic/SceneContext.hpp>
#include <Negibisu/AST/Command/CharacterCommand.hpp>
#include <Negibisu/AST/Command/ImageCommand.hpp>
#include <Negibisu/AST/Command/AudioCommand.hpp>

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
    // image
    NEGI_DEFINE_BUILTIN("changeImage", ImageChangeCommand),
    NEGI_DEFINE_BUILTIN("clearImage", ImageClearCommand),
    // character
    NEGI_DEFINE_BUILTIN("enter", CharacterEnterStageCommand),
    NEGI_DEFINE_BUILTIN("exit", CharacterExitStageCommand),
    NEGI_DEFINE_BUILTIN("move", CharacterMoveCommand),
    NEGI_DEFINE_BUILTIN("expr", CharacterChangeExpressionCommand),
    NEGI_DEFINE_BUILTIN("state", CharacterSetStateCommand),
    // audio
    NEGI_DEFINE_BUILTIN("playMusic", AudioPlayMusicCommand),
    NEGI_DEFINE_BUILTIN("stopMusic", AudioStopCommand),
    NEGI_DEFINE_BUILTIN("playSound", AudioPlaySoundEffectCommand),
};
#undef NEGI_DEFINE_BUILTIN
}

void IntrinsicCommand::parse(ParsingContext *ctx)
{
    USAGI_THROW(std::logic_error(
        "Intrinsic command must be constructed by explicitly providing "
        "arguments."));
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

void outputAvailableCommands()
{
    for(auto &&c : BUILTIN_COMMANDS)
    {
        fmt::print("{{{}", c.first);
        auto params = c.second()->parameterInfo();
        if(params.size())
        {
            fmt::print(":");
            std::size_t i = 0;
            for(auto &&p : params)
            {
                fmt::print("{}{}",
                    p.name, ++i == params.size() ? "}\n" : ","
                );
            }
        }
        else
        {
            fmt::print("}}\n");
        }
    }
}
}
