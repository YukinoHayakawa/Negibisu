#pragma once

#include <vector>
#include <optional>

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

class CharacterSetStateCommand : public CharacterCommand
{
    TokenRef mExpression;
    TokenRef mPosition;
    std::optional<CharacterChangeExpressionCommand> mCmdExpr;
    std::optional<CharacterMoveCommand> mCmdMove;

public:
    CharacterSetStateCommand() = default;
    CharacterSetStateCommand(
        TokenRef character,
        TokenRef expression,
        TokenRef position);

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

class CharacterAllExitStageCommand : public IntrinsicCommand
{
    std::vector<TokenRef> mCharacters;

public:
    CharacterAllExitStageCommand() = default;

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

class CharacterSetAliasCommand : public CharacterCommand
{
    TokenRef mAlias;

public:
    CharacterSetAliasCommand() = default;
    CharacterSetAliasCommand(
        TokenRef character,
        TokenRef alias);

    ParameterList parameterInfo() const override;

    void check(SceneContext *ctx) override;
    void generate(SceneContext *ctx) const override;
    void print(PrintContext &ctx) const override;
};

class CharacterRemoveAliasCommand : public CharacterCommand
{
public:
    using CharacterCommand::CharacterCommand;

    ParameterList parameterInfo() const override;

    void check(SceneContext *ctx) override;
    void generate(SceneContext *ctx) const override;
    void print(PrintContext &ctx) const override;
};

class CharacterTag : public StatementNode
{
    TokenRef mCharacter;
    TokenRef mAlias;
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
