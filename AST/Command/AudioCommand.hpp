#pragma once

#include <Negibisu/AST/Statement/IntrinsicCommand.hpp>

namespace usagi::negi
{
class AudioCommand : public IntrinsicCommand
{
public:
    void check(SceneContext *ctx) override;
};

class AudioPlayCommand : public AudioCommand
{
protected:
    TokenRef mAssetPath;

public:
    AudioPlayCommand() = default;
    explicit AudioPlayCommand(TokenRef asset_path);

    ParameterList parameterInfo() const override;
};

class AudioPlayMusicCommand : public AudioPlayCommand
{
public:
    using AudioPlayCommand::AudioPlayCommand;

    void generate(SceneContext *ctx) const override;
    void print(PrintContext &ctx) const override;
};

class AudioPlaySoundEffectCommand : public AudioPlayCommand
{
public:
    using AudioPlayCommand::AudioPlayCommand;

    void generate(SceneContext *ctx) const override;
    void print(PrintContext &ctx) const override;
};
}
