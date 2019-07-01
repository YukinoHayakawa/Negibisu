#pragma once

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

    std::initializer_list<CommandParameterInfo> parameterInfo() const override;
    void fillArguments(
        SceneContext *ctx,
        const std::vector<TokenRef> &args) override;

    void generate(SceneContext *ctx) override;
    void print(PrintContext &ctx) override;
};

class ImageClearCommand : public ImageCommand
{
public:
    using ImageCommand::ImageCommand;

    std::initializer_list<CommandParameterInfo> parameterInfo() const override;
    void fillArguments(
        SceneContext *ctx,
        const std::vector<TokenRef> &args) override;

    void generate(SceneContext *ctx) override;
    void print(PrintContext &ctx) override;
};
}
