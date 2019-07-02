#include "AudioCommand.hpp"

#include <Negibisu/Semantic/SceneContext.hpp>
#include <Negibisu/AST/PrintContext.hpp>

namespace usagi::negi
{
/*
 * AudioCommand
 */

void AudioCommand::check(SceneContext *ctx)
{
}

/*
 * AudioPlayCommand
 */

AudioPlayCommand::AudioPlayCommand(TokenRef asset_path)
    : mAssetPath(std::move(asset_path))
{
}

std::initializer_list<CommandParameterInfo>
AudioPlayCommand::parameterInfo() const
{
    static std::initializer_list<CommandParameterInfo> params = {
        { "assetPath", CommandParameterType::STRING },
    };
    return params;
}

void AudioPlayCommand::fillArguments(
    SceneContext *ctx,
    const std::vector<TokenRef> &args)
{
    IntrinsicCommand::fillArguments(ctx, args);

    mAssetPath = args[0];
}

/*
 * AudioPlayMusicCommand
 */

void AudioPlayMusicCommand::generate(SceneContext *ctx) const
{
    ctx->print(
        "sys:playMusic(\"{}\");",
        mAssetPath
    );
}

void AudioPlayMusicCommand::print(PrintContext &ctx) const
{
    ctx.print(
        "AUDIO_PLAY_MUSIC: asset=\"{}\"",
        mAssetPath
    );
}

/*
 * AudioPlaySoundEffectCommand
 */

void AudioPlaySoundEffectCommand::generate(SceneContext *ctx) const
{
    ctx->print(
        "sys:playSoundEffect(\"{}\");",
        mAssetPath
    );
}

void AudioPlaySoundEffectCommand::print(PrintContext &ctx) const
{
    ctx.print(
        "AUDIO_PLAY_SOUND_EFFECT: asset=\"{}\"",
        mAssetPath
    );
}
}
