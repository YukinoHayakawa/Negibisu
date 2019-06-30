#include "ImageCommand.hpp"

#include <Negibisu/Semantic/SceneContext.hpp>

namespace usagi::negi
{
void ImageCommand::check(SceneContext *ctx)
{
    CommandNode::check(ctx);

    ctx->symbol_table.lookup(mArgs[0].ref, SymbolType::IMAGE_LAYER);
}

std::initializer_list<CommandParameterInfo>
ImageChangeCommand::parameterInfo() const
{
    return {
        { "layerName", CommandParameterType::STRING },
        { "imageAssetName", CommandParameterType::STRING },
    };
}

void ImageChangeCommand::check(SceneContext *ctx)
{
    ImageCommand::check(ctx);
}

void ImageChangeCommand::generate(SceneContext *ctx)
{
    fmt::print(
        *ctx->output,
        "{0}:changeImage(\"{1}\");",
        ctx->symbol_table.lookup(
            mArgs[0].ref, SymbolType::IMAGE_LAYER
        ).object_name,
        mArgs[1]
    );
}

std::initializer_list<CommandParameterInfo>
ImageClearCommand::parameterInfo() const
{
    return {
        { "layerName", CommandParameterType::STRING }
    };
}

void ImageClearCommand::check(SceneContext *ctx)
{
    ImageCommand::check(ctx);
}

void ImageClearCommand::generate(SceneContext *ctx)
{
    fmt::print(
        *ctx->output,
        "{0}:changeImage();",
        ctx->symbol_table.lookup(
            mArgs[0].ref, SymbolType::IMAGE_LAYER
        ).object_name
    );
}
}
