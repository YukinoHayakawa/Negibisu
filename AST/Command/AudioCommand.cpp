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

ParameterList AudioPlayCommand::parameterInfo() const
{
    NEGI_RETURN_PARAMS(
        NEGI_PARAM("assetPath", STRING, mAssetPath),
    );
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
