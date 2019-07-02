#pragma once

#include <vector>

#include <Negibisu/AST/Statement/IntrinsicCommand.hpp>

namespace usagi::negi
{
class CharacterCommand : public IntrinsicCommand
{
protected:
    bool mGenerate = true;
    TokenRef mCharacter;

public:
    CharacterCommand() = default;
    explicit CharacterCommand(TokenRef character);

    void check(SceneContext *ctx) override;
};

class CharacterMoveCommand : public CharacterCommand
{
    TokenRef mPosition;

public:
    CharacterMoveCommand() = default;
    CharacterMoveCommand(TokenRef character, TokenRef position);

    ParameterList parameterInfo() const override;

    void check(SceneContext *ctx) override;
    void generate(SceneContext *ctx) const override;
    void print(PrintContext &ctx) const override;
};

class CharacterChangeExpressionCommand : public CharacterCommand
{
    TokenRef mExpression;

public:
    CharacterChangeExpressionCommand() = default;
    CharacterChangeExpressionCommand(TokenRef character, TokenRef expression);

    ParameterList parameterInfo() const override;

    void check(SceneContext *ctx) override;
    void generate(SceneContext *ctx) const override;
    void print(PrintContext &ctx) const override;
};

class CharacterEnterStageCommand : public CharacterCommand
{
    TokenRef mExpression;
    TokenRef mPosition;

public:
    CharacterEnterStageCommand() = default;
    CharacterEnterStageCommand(
        TokenRef character,
        TokenRef expression,
        TokenRef position);

    ParameterList parameterInfo() const override;

    void check(SceneContext *ctx) override;
    void generate(SceneContext *ctx) const override;
    void print(PrintContext &ctx) const override;
};

class CharacterExitStageCommand : public CharacterCommand
{
public:
    CharacterExitStageCommand() = default;
    explicit CharacterExitStageCommand(TokenRef character);

    ParameterList parameterInfo() const override;

    void check(SceneContext *ctx) override;
    void generate(SceneContext *ctx) const override;
    void print(PrintContext &ctx) const override;
};

class CharacterSayCommand : public CharacterCommand
{
protected:
    TokenRef mText;

public:
    CharacterSayCommand() = default;
    CharacterSayCommand(TokenRef character, TokenRef text);

    ParameterList parameterInfo() const override;

    void check(SceneContext *ctx) override;
    void generate(SceneContext *ctx) const override;
    void print(PrintContext &ctx) const override;
};

class CharacterSetDisguiseCommand : public CharacterCommand
{
    TokenRef mDisguisedName;

public:
    CharacterSetDisguiseCommand() = default;
    CharacterSetDisguiseCommand(
        TokenRef character,
        TokenRef disguised_name);

    ParameterList parameterInfo() const override;

    void check(SceneContext *ctx) override;
    void generate(SceneContext *ctx) const override;
    void print(PrintContext &ctx) const override;
};

class CharacterRemoveDisguiseCommand : public CharacterCommand
{
public:
    using CharacterCommand::CharacterCommand;

    ParameterList parameterInfo() const override;

    void check(SceneContext *ctx) override;
    void generate(SceneContext *ctx) const override;
    void print(PrintContext &ctx) const override;
};

class NarratorSayCommand : public IntrinsicCommand
{
    TokenRef mText;

public:
    NarratorSayCommand() = default;
    explicit NarratorSayCommand(TokenRef text);

    ParameterList parameterInfo() const override;

    void check(SceneContext *ctx) override;
    void generate(SceneContext *ctx) const override;
    void print(PrintContext &ctx) const override;
};

class CharacterTag : public StatementNode
{
    TokenRef mCharacter;
    TokenRef mDisguisedName;
    TokenRef mExpression;
    TokenRef mPosition;

    std::vector<std::unique_ptr<StatementNode>> mStatements;

public:
    void parse(ParsingContext *ctx) override;
    void check(SceneContext *ctx) override;
    void generate(SceneContext *ctx) const override;
    void print(PrintContext &ctx) const override;
};
}
