#include "ImageCommand.hpp"

#include <Negibisu/Semantic/SceneContext.hpp>
#include <Negibisu/AST/PrintContext.hpp>

namespace usagi::negi
{
/*
 * ImageCommand
 */

ImageCommand::ImageCommand(TokenRef image_layer_name)
    : mImageLayerName(std::move(image_layer_name))
{
}

void ImageCommand::check(SceneContext *ctx)
{
    ctx->symbol_table.lookup(mImageLayerName.ref, SymbolType::IMAGE_LAYER);
}

/*
 * ImageChangeCommand
 */

ImageChangeCommand::ImageChangeCommand(
    TokenRef image_layer_name,
    TokenRef asset_path)
    : ImageCommand(std::move(image_layer_name))
    , mAssetPath(std::move(asset_path))
{
}

std::initializer_list<CommandParameterInfo>
ImageChangeCommand::parameterInfo() const
{
    return {
        { "layerName", CommandParameterType::STRING },
        { "imageAssetName", CommandParameterType::STRING },
    };
}

void ImageChangeCommand::fillArguments(
    SceneContext *ctx,
    const std::vector<TokenRef> &args)
{
    IntrinsicCommand::fillArguments(ctx, args);

    mImageLayerName = args[0];
    mAssetPath = args[1];
}

void ImageChangeCommand::generate(SceneContext *ctx)
{
    ctx->print(
        "{0}:changeImage(\"{1}\");",
        ctx->symbol_table.lookup(
            mImageLayerName.ref, SymbolType::IMAGE_LAYER
        ).object_name,
        mAssetPath
    );
}

void ImageChangeCommand::print(PrintContext &ctx)
{
    ctx.print(
        "IMAGE_CHANGE: layer=\"{}\", asset=\"{}\"",
        mImageLayerName, mAssetPath
    );
}

/*
 * ImageClearCommand
 */

std::initializer_list<CommandParameterInfo>
ImageClearCommand::parameterInfo() const
{
    return {
        { "layerName", CommandParameterType::STRING }
    };
}

void ImageClearCommand::fillArguments(
    SceneContext *ctx,
    const std::vector<TokenRef> &args)
{
    IntrinsicCommand::fillArguments(ctx, args);

    mImageLayerName = args[0];
}

void ImageClearCommand::generate(SceneContext *ctx)
{
    ctx->print(
        "{0}:changeImage();",
        ctx->symbol_table.lookup(
            mImageLayerName.ref, SymbolType::IMAGE_LAYER
        ).object_name
    );
}

void ImageClearCommand::print(PrintContext &ctx)
{
    ctx.print(
        "IMAGE_CLEAR: layer=\"{}\"",
        mImageLayerName
    );
}
}
