#include "CharacterCommand.hpp"

#include <Negibisu/Semantic/SceneContext.hpp>

namespace usagi::negi
{
/*
 * CharacterCommand
 */

void CharacterCommand::check(SceneContext *ctx)
{
    CommandNode::check(ctx);

    ctx->symbol_table.lookup(mArgs[0].ref, SymbolType::CHARACTER);
}

/*
 * CharacterEnterStageCommand
 */

std::initializer_list<CommandParameterInfo>
CharacterEnterStageCommand::parameterInfo() const
{
    return {
        { "characterName", CommandParameterType::STRING }
    };
}

void CharacterEnterStageCommand::check(SceneContext *ctx)
{
    CharacterCommand::check(ctx);
    auto &state = ctx->characterState(mArgs[0]);
    mGenerate = state.on_stage == false;
    state.on_stage = true;
}

void CharacterEnterStageCommand::generate(SceneContext *ctx)
{
    if(mGenerate)
    {
        fmt::print(
            *ctx->output,
            "{0}:exitStage();",
            ctx->symbol_table.lookup(
                mArgs[0].ref, SymbolType::CHARACTER
            ).object_name
        );
    }
}

/*
 * CharacterExitStageCommand
 */

std::initializer_list<CommandParameterInfo>
CharacterExitStageCommand::parameterInfo() const
{
    return {
        { "characterName", CommandParameterType::STRING }
    };
}

void CharacterExitStageCommand::check(SceneContext *ctx)
{
    CharacterCommand::check(ctx);
    auto &state = ctx->checkCharacterOnStage(mArgs[0]);
    state.on_stage = false;
}

void CharacterExitStageCommand::generate(SceneContext *ctx)
{
    fmt::print(
        *ctx->output,
        "{0}:exitStage();",
        ctx->symbol_table.lookup(
            mArgs[0].ref, SymbolType::CHARACTER
        ).object_name
    );
}

/*
 * CharacterMoveCommand
 */

std::initializer_list<CommandParameterInfo>
CharacterMoveCommand::parameterInfo() const
{
    return {
        { "characterName", CommandParameterType::STRING },
        { "positionName", CommandParameterType::STRING },
    };
}

void CharacterMoveCommand::check(SceneContext *ctx)
{
    CharacterCommand::check(ctx);
    ctx->symbol_table.lookup(mArgs[1].ref, SymbolType::POSITION);

    auto &state = ctx->checkCharacterOnStage(mArgs[0]);
    mGenerate = state.current_position != mArgs[1]->text;
    state.current_position = mArgs[1]->text;
}

void CharacterMoveCommand::generate(SceneContext *ctx)
{
    if(mGenerate)
    {
        fmt::print(
            *ctx->output,
            "{0}:changePosition(\"{1}\");",
            ctx->symbol_table.lookup(
                mArgs[0].ref, SymbolType::CHARACTER
            ).object_name,
            ctx->symbol_table.lookup(
                mArgs[1].ref, SymbolType::POSITION
            ).object_name
        );
    }
}

/*
 * CharacterChangeExpressionCommand
 */

std::initializer_list<CommandParameterInfo>
CharacterChangeExpressionCommand::parameterInfo() const
{
    return {
        { "characterName", CommandParameterType::STRING },
        { "expressionName", CommandParameterType::STRING },
    };
}

void CharacterChangeExpressionCommand::check(SceneContext *ctx)
{
    CharacterCommand::check(ctx);
    ctx->symbol_table.lookup(mArgs[1].ref, SymbolType::EXPRESSION);

    auto &state = ctx->checkCharacterOnStage(mArgs[0]);
    mGenerate = state.current_expression != mArgs[1]->text;
    state.current_expression = mArgs[1]->text;
}

void CharacterChangeExpressionCommand::generate(SceneContext *ctx)
{
    if(mGenerate)
    {
        fmt::print(
            *ctx->output,
            "{0}:changeExpression(\"{1}\");",
            ctx->symbol_table.lookup(
                mArgs[0].ref, SymbolType::CHARACTER
            ).object_name,
            ctx->symbol_table.lookup(
                mArgs[1].ref, SymbolType::EXPRESSION
            ).object_name
        );
    }
}
}
