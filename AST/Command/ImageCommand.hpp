﻿#pragma once

#include <Negibisu/AST/Statement/IntrinsicCommand.hpp>

namespace usagi::negi
{
class ImageCommand : public IntrinsicCommand
{
protected:
    TokenRef mImageLayerName;

public:
    ImageCommand() = default;
    explicit ImageCommand(TokenRef image_layer_name);

    void check(SceneContext *ctx) override;
};

class ImageChangeCommand : public ImageCommand
{
    TokenRef mAssetPath;

public:
    ImageChangeCommand() = default;
    ImageChangeCommand(TokenRef image_layer_name, TokenRef asset_path);

    ParameterList parameterInfo() const override;

    void generate(SceneContext *ctx) const override;
    void check(SceneContext *ctx) override;
    void print(PrintContext &ctx) const override;
};

class ImageClearCommand : public ImageCommand
{
public:
    using ImageCommand::ImageCommand;

    ParameterList parameterInfo() const override;

    void generate(SceneContext *ctx) const override;
    void print(PrintContext &ctx) const override;
};
}
