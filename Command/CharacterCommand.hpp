#pragma once

#include <Negibisu/AST/CommandNode.hpp>

namespace usagi::negi
{
class CharacterCommand : public CommandNode
{
protected:
    bool mGenerate = true;

public:
    using CommandNode::CommandNode;

    void check(SceneContext *ctx) override;
};

class CharacterEnterStageCommand : public CharacterCommand
{
public:
    using CharacterCommand::CharacterCommand;

    std::initializer_list<CommandParameterInfo> parameterInfo() const override;
    void check(SceneContext *ctx) override;
    void generate(SceneContext *ctx) override;
};

class CharacterExitStageCommand : public CharacterCommand
{
public:
    using CharacterCommand::CharacterCommand;

    std::initializer_list<CommandParameterInfo> parameterInfo() const override;
    void check(SceneContext *ctx) override;
    void generate(SceneContext *ctx) override;
};

class CharacterMoveCommand : public CharacterCommand
{
public:
    using CharacterCommand::CharacterCommand;

    std::initializer_list<CommandParameterInfo> parameterInfo() const override;
    void check(SceneContext *ctx) override;
    void generate(SceneContext *ctx) override;
};

class CharacterChangeExpressionCommand : public CharacterCommand
{
public:
    using CharacterCommand::CharacterCommand;

    std::initializer_list<CommandParameterInfo> parameterInfo() const override;
    void check(SceneContext *ctx) override;
    void generate(SceneContext *ctx) override;
};
}
