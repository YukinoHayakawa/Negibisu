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

ParameterList ImageChangeCommand::parameterInfo() const
{
    NEGI_RETURN_PARAMS(
        NEGI_PARAM("layerName", STRING, mImageLayerName),
        NEGI_PARAM("imageAssetPath", STRING, mAssetPath),
    );
}

void ImageChangeCommand::generate(SceneContext *ctx) const
{
    ctx->print(
        "{0}:changeImage(\"{1}\");",
        ctx->symbol_table.lookup(
            mImageLayerName.ref, SymbolType::IMAGE_LAYER
        ).object_name,
        mAssetPath
    );
}

void ImageChangeCommand::print(PrintContext &ctx) const
{
    ctx.print(
        "IMAGE_CHANGE: layer=\"{}\", asset=\"{}\"",
        mImageLayerName, mAssetPath
    );
}

/*
 * ImageClearCommand
 */

ParameterList ImageClearCommand::parameterInfo() const
{
    NEGI_RETURN_PARAMS(
        NEGI_PARAM("layerName", STRING, mImageLayerName),
    );
}

void ImageClearCommand::generate(SceneContext *ctx) const
{
    ctx->print(
        "{0}:changeImage();",
        ctx->symbol_table.lookup(
            mImageLayerName.ref, SymbolType::IMAGE_LAYER
        ).object_name
    );
}

void ImageClearCommand::print(PrintContext &ctx) const
{
    ctx.print(
        "IMAGE_CLEAR: layer=\"{}\"",
        mImageLayerName
    );
}
}
