#include "CharacterCommand.hpp"

#include <Negibisu/Semantic/SceneContext.hpp>
#include <Negibisu/AST/PrintContext.hpp>
#include <Negibisu/Parsing/ParsingContext.hpp>

namespace usagi::negi
{
/*
 * CharacterCommand
 */

CharacterCommand::CharacterCommand(TokenRef character)
    : mCharacter(std::move(character))
{
}

void CharacterCommand::check(SceneContext *ctx)
{
    ctx->symbol_table.lookup(mCharacter, SymbolType::CHARACTER);
}

/*
 * CharacterMoveCommand
 */

CharacterMoveCommand::CharacterMoveCommand(
    TokenRef character,
    TokenRef position)
    : CharacterCommand(std::move(character))
    , mPosition(std::move(position))
{
}

std::initializer_list<CommandParameterInfo>
CharacterMoveCommand::parameterInfo() const
{
    return {
        { "characterName", CommandParameterType::STRING },
        { "positionName", CommandParameterType::STRING },
    };
}

void CharacterMoveCommand::fillArguments(
    SceneContext *ctx,
    const std::vector<TokenRef> &args)
{
    IntrinsicCommand::fillArguments(ctx, args);

    mCharacter = args[0];
    mPosition = args[1];
}

void CharacterMoveCommand::check(SceneContext *ctx)
{
    CharacterCommand::check(ctx);

    ctx->symbol_table.lookup(mPosition, SymbolType::POSITION);

    auto &state = ctx->ensureCharacterOnStage(mCharacter);
    mGenerate = state.current_position != mPosition->text;
    state.current_position = mPosition->text;
}

void CharacterMoveCommand::generate(SceneContext *ctx)
{
    if(mGenerate)
    {
        ctx->print(
            "{0}:changePosition(\"{1}\");",
            ctx->symbol_table.lookup(
                mCharacter, SymbolType::CHARACTER
            ).object_name,
            ctx->symbol_table.lookup(
                mPosition, SymbolType::POSITION
            ).object_name
        );
    }
}

void CharacterMoveCommand::print(PrintContext &ctx)
{
    ctx.print("CHARACTER_MOVE: char=\"{}\", pos=\"{}\"",
        mCharacter, mPosition);
}

/*
 * CharacterChangeExpressionCommand
 */

CharacterChangeExpressionCommand::CharacterChangeExpressionCommand(
    TokenRef character,
    TokenRef expression)
    : CharacterCommand(std::move(character))
    , mExpression(std::move(expression))
{
}

std::initializer_list<CommandParameterInfo>
CharacterChangeExpressionCommand::parameterInfo() const
{
    return {
        { "characterName", CommandParameterType::STRING },
        { "expressionName", CommandParameterType::STRING },
    };
}

void CharacterChangeExpressionCommand::fillArguments(
    SceneContext *ctx,
    const std::vector<TokenRef> &args)
{
    IntrinsicCommand::fillArguments(ctx, args);

    mCharacter = args[0];
    mExpression = args[1];
}

void CharacterChangeExpressionCommand::check(SceneContext *ctx)
{
    CharacterCommand::check(ctx);

    ctx->symbol_table.lookup(mExpression, SymbolType::EXPRESSION);

    auto &state = ctx->ensureCharacterOnStage(mCharacter);
    mGenerate = state.current_expression != mExpression->text;
    state.current_expression = mExpression->text;
}

void CharacterChangeExpressionCommand::generate(SceneContext *ctx)
{
    if(mGenerate)
    {
        ctx->print(
            "{0}:changeExpression(\"{1}\");",
            ctx->symbol_table.lookup(
                mCharacter, SymbolType::CHARACTER
            ).object_name,
            ctx->symbol_table.lookup(
                mExpression, SymbolType::EXPRESSION
            ).object_name
        );
    }
}

void CharacterChangeExpressionCommand::print(PrintContext &ctx)
{
    ctx.print("CHARACTER_EXPR: char=\"{}\", expr=\"{}\"",
        mCharacter, mExpression);
}

/*
 * CharacterEnterStageCommand
 */

CharacterEnterStageCommand::CharacterEnterStageCommand(
    TokenRef character,
    TokenRef expression,
    TokenRef position)
    : CharacterCommand(std::move(character))
    , mExpression(std::move(expression))
    , mPosition(std::move(position))
{
}

std::initializer_list<CommandParameterInfo>
CharacterEnterStageCommand::parameterInfo() const
{
    return {
        { "characterName", CommandParameterType::STRING }
    };
}

void CharacterEnterStageCommand::fillArguments(
    SceneContext *ctx,
    const std::vector<TokenRef> &args)
{
    IntrinsicCommand::fillArguments(ctx, args);

    mCharacter = args[0];
    mExpression = args[1];
    mPosition = args[2];
}

void CharacterEnterStageCommand::check(SceneContext *ctx)
{
    CharacterCommand::check(ctx);

    ctx->symbol_table.lookup(mPosition, SymbolType::POSITION);
    ctx->symbol_table.lookup(mExpression, SymbolType::EXPRESSION);

    auto &state = ctx->characterState(mCharacter);
    if(state.on_stage)
    {
        ctx->semanticError(mCharacter,
            "Character {} already on the stage.",
            mCharacter);
    }
    state.on_stage = true;
}

void CharacterEnterStageCommand::generate(SceneContext *ctx)
{
    ctx->print(
        "{0}:enterStage({1}, {2});",
        ctx->symbol_table.lookup(
            mCharacter, SymbolType::CHARACTER
        ).object_name,
        ctx->symbol_table.lookup(
            mExpression, SymbolType::EXPRESSION
        ).object_name,
        ctx->symbol_table.lookup(
            mPosition, SymbolType::POSITION
        ).object_name
    );
}

void CharacterEnterStageCommand::print(PrintContext &ctx)
{
    ctx.print("CHARACTER_ENTER: char=\"{}\", expr=\"{}\", pos=\"{}\"",
        mCharacter, mExpression, mPosition);
}

/*
 * CharacterExitStageCommand
 */

CharacterExitStageCommand::CharacterExitStageCommand(TokenRef character)
    : CharacterCommand(std::move(character))
{
}

std::initializer_list<CommandParameterInfo>
CharacterExitStageCommand::parameterInfo() const
{
    return {
        { "characterName", CommandParameterType::STRING }
    };
}

void CharacterExitStageCommand::fillArguments(
    SceneContext *ctx,
    const std::vector<TokenRef> &args)
{
    IntrinsicCommand::fillArguments(ctx, args);

    mCharacter = args[0];
}

void CharacterExitStageCommand::check(SceneContext *ctx)
{
    CharacterCommand::check(ctx);

    auto &state = ctx->ensureCharacterOnStage(mCharacter);
    state.on_stage = false;
}

void CharacterExitStageCommand::generate(SceneContext *ctx)
{
    ctx->print(
        "{0}:exitStage();",
        ctx->symbol_table.lookup(
            mCharacter, SymbolType::CHARACTER
        ).object_name
    );
}

void CharacterExitStageCommand::print(PrintContext &ctx)
{
    ctx.print("CHARACTER_EXIT: char=\"{}\"",
        mCharacter);
}

/*
 * CharacterSayCommand
 */

CharacterSayCommand::CharacterSayCommand(TokenRef character, TokenRef text)
    : CharacterCommand(std::move(character))
    , mText(std::move(text))
{
}

std::initializer_list<CommandParameterInfo>
CharacterSayCommand::parameterInfo() const
{
    return {
        { "characterName", CommandParameterType::STRING },
        { "text", CommandParameterType::STRING },
    };
}

void CharacterSayCommand::fillArguments(
    SceneContext *ctx,
    const std::vector<TokenRef> &args)
{
    IntrinsicCommand::fillArguments(ctx, args);

    mCharacter = args[0];
    mText = args[1];
}

void CharacterSayCommand::check(SceneContext *ctx)
{
    CharacterCommand::check(ctx);

    ctx->symbol_table.addStringLiteral(mText);
}

void CharacterSayCommand::generate(SceneContext *ctx)
{
    ctx->print(
        "{0}:say(\"{1}\");",
        ctx->symbol_table.lookup(
            mCharacter, SymbolType::CHARACTER
        ).object_name,
        mText
    );
}

void CharacterSayCommand::print(PrintContext &ctx)
{
    ctx.print("CHARACTER_SAY: char=\"{}\", text=\"{}\"",
        mCharacter, mText);
}

/*
 * CharacterSetDisguiseCommand
 */

CharacterSetDisguiseCommand::CharacterSetDisguiseCommand(
    TokenRef character,
    TokenRef disguised_name)
    : CharacterCommand(std::move(character))
    , mDisguisedName(std::move(disguised_name))
{
}

std::initializer_list<CommandParameterInfo>
CharacterSetDisguiseCommand::parameterInfo() const
{
    return {
        { "characterName", CommandParameterType::STRING },
        { "disguisedName", CommandParameterType::STRING },
    };
}

void CharacterSetDisguiseCommand::fillArguments(
    SceneContext *ctx,
    const std::vector<TokenRef> &args)
{
    IntrinsicCommand::fillArguments(ctx, args);

    mCharacter = args[0];
    mDisguisedName = args[1];
}

void CharacterSetDisguiseCommand::check(SceneContext *ctx)
{
    CharacterCommand::check(ctx);

    auto &state = ctx->characterState(mCharacter);
    mGenerate = state.disguised_name != mDisguisedName->text;
    state.disguised_name = mDisguisedName->text;

    ctx->symbol_table.addStringLiteral(mDisguisedName);
}

void CharacterSetDisguiseCommand::generate(SceneContext *ctx)
{
    if(mGenerate)
    {
        ctx->print(
            "{0}:setDisguise(\"{1}\");",
            ctx->symbol_table.lookup(
                mCharacter, SymbolType::CHARACTER
            ).object_name,
            mDisguisedName
        );
    }
}

void CharacterSetDisguiseCommand::print(PrintContext &ctx)
{
    ctx.print("CHARACTER_SET_DISGUISE: "
        "char=\"{}\", disguise=\"{}\"",
        mCharacter, mDisguisedName);
}

/*
 * CharacterRemoveDisguiseCommand
 */

std::initializer_list<CommandParameterInfo>
CharacterRemoveDisguiseCommand::parameterInfo() const
{
    return {
        { "characterName", CommandParameterType::STRING },
    };
}

void CharacterRemoveDisguiseCommand::fillArguments(
    SceneContext *ctx,
    const std::vector<TokenRef> &args)
{
    IntrinsicCommand::fillArguments(ctx, args);

    mCharacter = args[0];
}

void CharacterRemoveDisguiseCommand::check(SceneContext *ctx)
{
    CharacterCommand::check(ctx);

    auto &state = ctx->characterState(mCharacter);
    mGenerate = state.disguised_name != "";
}

void CharacterRemoveDisguiseCommand::generate(SceneContext *ctx)
{
    if(mGenerate)
    {
        ctx->print(
            "{0}:removeDisguise();",
            ctx->symbol_table.lookup(
                mCharacter, SymbolType::CHARACTER
            ).object_name
        );
    }
}

void CharacterRemoveDisguiseCommand::print(PrintContext &ctx)
{
    ctx.print("CHARACTER_REMOVE_DISGUISE: "
        "char=\"{}\"",
        mCharacter);
}

/*
 * NarratorSayCommand
 */

NarratorSayCommand::NarratorSayCommand(TokenRef text)
    : mText(std::move(text))
{
}

std::initializer_list<CommandParameterInfo>
NarratorSayCommand::parameterInfo() const
{
    return {
        { "text", CommandParameterType::STRING },
    };
}

void NarratorSayCommand::fillArguments(
    SceneContext *ctx,
    const std::vector<TokenRef> &args)
{
    IntrinsicCommand::fillArguments(ctx, args);

    mText = args[0];
}

void NarratorSayCommand::check(SceneContext *ctx)
{
    ctx->symbol_table.addStringLiteral(mText);
}

void NarratorSayCommand::generate(SceneContext *ctx)
{
    ctx->print(
        "narrator:say(\"{}\");",
        mText
    );
}

void NarratorSayCommand::print(PrintContext &ctx)
{
    ctx.print("NARRATOR_SAY: text=\"{}\"",
        mText);
}

/*
 * CharacterTag
 */

void CharacterTag::parse(ParsingContext *ctx)
{
    // expected to be the first token of the line.
    if(!ctx->line.beginning)
    {
        ctx->syntaxError(
            "Character tag can only be put at "
            "the beginning of a line."
        );
    }
    // parse character tag
    ctx->advance();
    mCharacter = ctx->consumeString();
    // optional alias name
    if(ctx->currentType() == TokenType::EQUAL)
    {
        ctx->advance();
        mDisguisedName = mCharacter;
        mCharacter = ctx->consumeString();
    }
    // optional expression and position change
    if(ctx->currentType() == TokenType::COMMA)
    {
        ctx->advance();
        mExpression = ctx->consumeString();
        ctx->consume(TokenType::COMMA);
        mPosition = ctx->consumeString();
    }
    ctx->consume(TokenType::RIGHT_BRACKET);
    ctx->line.current_character = mCharacter;
}

void CharacterTag::check(SceneContext *ctx)
{
    auto &state = ctx->characterState(mCharacter);

    if(!mDisguisedName)
    {
        if(!state.disguised_name.empty())
        {
            mStatements.push_back(
                std::make_unique<CharacterRemoveDisguiseCommand>(
                    mCharacter
                ));
            mStatements.back()->check(ctx);
        }
    }
    else if(state.disguised_name != mDisguisedName->text)
    {
        mStatements.push_back(
            std::make_unique<CharacterSetDisguiseCommand>(
                mCharacter, mDisguisedName
            ));
        mStatements.back()->check(ctx);
    }
    if(mExpression || mPosition)
    {
        assert(mExpression && mPosition);
        if(!state.on_stage)
        {
            mStatements.push_back(
                std::make_unique<CharacterEnterStageCommand>(
                    mCharacter, mExpression, mPosition
                ));
            mStatements.back()->check(ctx);
        }
        else
        {
            mStatements.push_back(
                std::make_unique<CharacterChangeExpressionCommand>(
                    mCharacter, mExpression
                ));
            mStatements.back()->check(ctx);
            mStatements.push_back(
                std::make_unique<CharacterMoveCommand>(
                    mCharacter, mPosition
                ));
            mStatements.back()->check(ctx);
        }
    }
}

void CharacterTag::generate(SceneContext *ctx)
{
    for(auto &&s : mStatements)
        s->generate(ctx);
}

void CharacterTag::print(PrintContext &ctx)
{
    ctx.print(
        "CHARACTER_TAG: char=\"{}\", alias=\"{}\", "
        "expr=\"{}\", pos=\"{}\"",
        mCharacter,
        mDisguisedName,
        mExpression,
        mPosition
    );
    ctx.push();
    for(auto &&s : mStatements)
        s->print(ctx);
    ctx.pop();
}
}
