#include "AudioCommand.hpp"

#include <Negibisu/Semantic/SceneContext.hpp>
#include <Negibisu/AST/PrintContext.hpp>

namespace usagi::negi
{
/*
 * AudioCommand
 */

AudioCommand::AudioCommand(TokenRef audio_track)
    : mAudioTrack(std::move(audio_track))
{
}

void AudioCommand::check(SceneContext *ctx)
{
    ctx->symbol_table.lookup(mAudioTrack, SymbolType::AUDIO_TRACK);
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
        NEGI_PARAM("audioTrack", STRING, mAudioTrack),
        NEGI_PARAM("assetPath", STRING, mAssetPath),
    );
}

void AudioPlayCommand::check(SceneContext *ctx)
{
    AudioCommand::check(ctx);

    ctx->symbol_table.addAssetRef(AssetType::AUDIO, mAssetPath);
}

/*
 * AudioStopCommand
 */

AudioStopCommand::AudioStopCommand(TokenRef asset_path)
    : mAssetPath(std::move(asset_path))
{
}

ParameterList AudioStopCommand::parameterInfo() const
{
    NEGI_RETURN_PARAMS(
        NEGI_PARAM("audioTrack", STRING, mAudioTrack),
    );
}

void AudioStopCommand::generate(SceneContext *ctx) const
{
    ctx->print(
        "{0}:stop();",
        ctx->symbol_table.lookup(
            mAudioTrack, SymbolType::AUDIO_TRACK
        ).object_name
    );
}

void AudioStopCommand::print(PrintContext &ctx) const
{
    ctx.print(
        "AUDIO_STOP_MUSIC"
    );
}

/*
 * AudioPlayMusicCommand
 */

void AudioPlayMusicCommand::generate(SceneContext *ctx) const
{
    ctx->print(
        "{0}:play(\"{1}\", true);",
        ctx->symbol_table.lookup(
            mAudioTrack, SymbolType::AUDIO_TRACK
        ).object_name,
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
        "{0}:play(\"{1}\", false);",
        ctx->symbol_table.lookup(
            mAudioTrack, SymbolType::AUDIO_TRACK
        ).object_name,
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
