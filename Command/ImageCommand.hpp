#pragma once

#include <Negibisu/AST/CommandNode.hpp>

namespace usagi::negi
{
class ImageCommand : public CommandNode
{
public:
    using CommandNode::CommandNode;

    void check(SceneContext *ctx) override = 0;
};

class ImageChangeCommand : public ImageCommand
{
public:
    using ImageCommand::ImageCommand;

    std::initializer_list<CommandParameterInfo> parameterInfo() const override;
    void check(SceneContext *ctx) override;
    void generate(SceneContext *ctx) override;
};

class ImageClearCommand : public ImageCommand
{
public:
    using ImageCommand::ImageCommand;

    std::initializer_list<CommandParameterInfo> parameterInfo() const override;
    void check(SceneContext *ctx) override;
    void generate(SceneContext *ctx) override;
};
}
