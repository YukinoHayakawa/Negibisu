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
    ctx->symbol_table.addAssetRef(AssetType::CHARACTER, mCharacter);
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

ParameterList CharacterMoveCommand::parameterInfo() const
{
    NEGI_RETURN_PARAMS(
        NEGI_PARAM("characterName", STRING, mCharacter),
        NEGI_PARAM("positionName", STRING, mPosition),
    );
}

void CharacterMoveCommand::check(SceneContext *ctx)
{
    CharacterCommand::check(ctx);

    ctx->symbol_table.lookup(mPosition, SymbolType::POSITION);

    auto &state = ctx->ensureCharacterOnStage(mCharacter);
    mGenerate = state.current_position != mPosition->text;
    state.current_position = mPosition->text;
}

void CharacterMoveCommand::generate(SceneContext *ctx) const
{
    if(mGenerate)
    {
        ctx->print(
            "{0}:changePosition({1});",
            ctx->symbol_table.lookup(
                mCharacter, SymbolType::CHARACTER
            ).object_name,
            ctx->symbol_table.lookup(
                mPosition, SymbolType::POSITION
            ).object_name
        );
    }
}

void CharacterMoveCommand::print(PrintContext &ctx) const
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

ParameterList CharacterChangeExpressionCommand::parameterInfo() const
{
    NEGI_RETURN_PARAMS(
        NEGI_PARAM("characterName", STRING, mCharacter),
        NEGI_PARAM("expressionName", STRING, mExpression),
    );
}

void CharacterChangeExpressionCommand::check(SceneContext *ctx)
{
    CharacterCommand::check(ctx);

    ctx->symbol_table.lookup(mExpression, SymbolType::EXPRESSION);

    auto &state = ctx->ensureCharacterOnStage(mCharacter);
    mGenerate = state.current_expression != mExpression->text;
    state.current_expression = mExpression->text;
}

void CharacterChangeExpressionCommand::generate(SceneContext *ctx) const
{
    if(mGenerate)
    {
        ctx->print(
            "{0}:changeExpression({1});",
            ctx->symbol_table.lookup(
                mCharacter, SymbolType::CHARACTER
            ).object_name,
            ctx->symbol_table.lookup(
                mExpression, SymbolType::EXPRESSION
            ).object_name
        );
    }
}

void CharacterChangeExpressionCommand::print(PrintContext &ctx) const
{
    ctx.print("CHARACTER_EXPR: char=\"{}\", expr=\"{}\"",
        mCharacter, mExpression);
}

/*
* CharacterSetStateCommand
*/

CharacterSetStateCommand::CharacterSetStateCommand(
    TokenRef character,
    TokenRef expression,
    TokenRef position)
    : CharacterCommand(std::move(character))
    , mExpression(std::move(expression))
    , mPosition(std::move(position))
{
}

ParameterList CharacterSetStateCommand::parameterInfo() const
{
    NEGI_RETURN_PARAMS(
        NEGI_PARAM("characterName", STRING, mCharacter),
        NEGI_PARAM("expressionName", STRING, mExpression),
        NEGI_PARAM("positionName", STRING, mPosition),
    );
}

void CharacterSetStateCommand::check(SceneContext *ctx)
{
    CharacterCommand::check(ctx);

    mCmdExpr.emplace(mCharacter, mExpression);
    mCmdMove.emplace(mCharacter, mPosition);
    mCmdExpr->check(ctx);
    mCmdMove->check(ctx);
}

void CharacterSetStateCommand::generate(SceneContext *ctx) const
{
    mCmdExpr->generate(ctx);
    mCmdMove->generate(ctx);
}

void CharacterSetStateCommand::print(PrintContext &ctx) const
{
    ctx.print("CHARACTER_STATE: char=\"{}\", expr=\"{}\", pos=\"{}\"",
        mCharacter, mExpression, mPosition);
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

ParameterList CharacterEnterStageCommand::parameterInfo() const
{
    NEGI_RETURN_PARAMS(
        NEGI_PARAM("characterName", STRING, mCharacter),
        NEGI_PARAM("expressionName", STRING, mExpression),
        NEGI_PARAM("positionName", STRING, mPosition),
    );
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

void CharacterEnterStageCommand::generate(SceneContext *ctx) const
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

void CharacterEnterStageCommand::print(PrintContext &ctx) const
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

ParameterList CharacterExitStageCommand::parameterInfo() const
{
    NEGI_RETURN_PARAMS(
        NEGI_PARAM("characterName", STRING, mCharacter),
    );
}

void CharacterExitStageCommand::check(SceneContext *ctx)
{
    CharacterCommand::check(ctx);

    auto &state = ctx->ensureCharacterOnStage(mCharacter);
    state.on_stage = false;
}

void CharacterExitStageCommand::generate(SceneContext *ctx) const
{
    ctx->print(
        "{0}:exitStage();",
        ctx->symbol_table.lookup(
            mCharacter, SymbolType::CHARACTER
        ).object_name
    );
}

void CharacterExitStageCommand::print(PrintContext &ctx) const
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

ParameterList CharacterSayCommand::parameterInfo() const
{
    NEGI_RETURN_PARAMS(
        NEGI_PARAM("characterName", STRING, mCharacter),
        NEGI_PARAM("text", STRING, mText),
    );
}

void CharacterSayCommand::check(SceneContext *ctx)
{
    CharacterCommand::check(ctx);

    ctx->symbol_table.addStringLiteral(mText);
}

void CharacterSayCommand::generate(SceneContext *ctx) const
{
    ctx->print(
        "{0}:say(\"{1}\");",
        ctx->symbol_table.lookup(
            mCharacter, SymbolType::CHARACTER
        ).object_name,
        mText
    );
}

void CharacterSayCommand::print(PrintContext &ctx) const
{
    ctx.print("CHARACTER_SAY: char=\"{}\", text=\"{}\"",
        mCharacter, mText);
}

/*
 * CharacterSetAliasCommand
 */

CharacterSetAliasCommand::CharacterSetAliasCommand(
    TokenRef character,
    TokenRef alias)
    : CharacterCommand(std::move(character))
    , mAlias(std::move(alias))
{
}

ParameterList CharacterSetAliasCommand::parameterInfo() const
{
    NEGI_RETURN_PARAMS(
        NEGI_PARAM("characterName", STRING, mCharacter),
        NEGI_PARAM("alias", STRING, mAlias),
    );
}

void CharacterSetAliasCommand::check(SceneContext *ctx)
{
    CharacterCommand::check(ctx);

    auto &state = ctx->characterState(mCharacter);
    mGenerate = state.alias != mAlias->text;
    state.alias = mAlias->text;

    ctx->symbol_table.addStringLiteral(mAlias);
}

void CharacterSetAliasCommand::generate(SceneContext *ctx) const
{
    if(mGenerate)
    {
        ctx->print(
            "{0}:setAlias(\"{1}\");",
            ctx->symbol_table.lookup(
                mCharacter, SymbolType::CHARACTER
            ).object_name,
            mAlias
        );
    }
}

void CharacterSetAliasCommand::print(PrintContext &ctx) const
{
    ctx.print("CHARACTER_SET_ALIAS: "
        "char=\"{}\", alias=\"{}\"",
        mCharacter, mAlias);
}

/*
 * CharacterRemoveAliasCommand
 */

ParameterList CharacterRemoveAliasCommand::parameterInfo() const
{
    NEGI_RETURN_PARAMS(
        NEGI_PARAM("characterName", STRING, mCharacter),
    );
}

void CharacterRemoveAliasCommand::check(SceneContext *ctx)
{
    CharacterCommand::check(ctx);

    auto &state = ctx->characterState(mCharacter);
    mGenerate = state.alias != "";
    state.alias = { };
}

void CharacterRemoveAliasCommand::generate(SceneContext *ctx) const
{
    if(mGenerate)
    {
        ctx->print(
            "{0}:removeAlias();",
            ctx->symbol_table.lookup(
                mCharacter, SymbolType::CHARACTER
            ).object_name
        );
    }
}

void CharacterRemoveAliasCommand::print(PrintContext &ctx) const
{
    ctx.print("CHARACTER_REMOVE_ALIAS: "
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

ParameterList NarratorSayCommand::parameterInfo() const
{
    NEGI_RETURN_PARAMS(
        NEGI_PARAM("text", STRING, mText),
    );
}

void NarratorSayCommand::check(SceneContext *ctx)
{
    ctx->symbol_table.addStringLiteral(mText);
}

void NarratorSayCommand::generate(SceneContext *ctx) const
{
    ctx->print(
        "narrator:say(\"{}\");",
        mText
    );
}

void NarratorSayCommand::print(PrintContext &ctx) const
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
        mAlias = mCharacter;
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

    if(!mAlias)
    {
        if(!state.alias.empty())
        {
            mStatements.push_back(
                std::make_unique<CharacterRemoveAliasCommand>(
                    mCharacter
                ));
            mStatements.back()->check(ctx);
        }
    }
    else if(state.alias != mAlias->text)
    {
        mStatements.push_back(
            std::make_unique<CharacterSetAliasCommand>(
                mCharacter, mAlias
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

void CharacterTag::generate(SceneContext *ctx) const
{
    for(auto &&s : mStatements)
        s->generate(ctx);
}

void CharacterTag::print(PrintContext &ctx) const
{
    ctx.print(
        "CHARACTER_TAG: char=\"{}\", alias=\"{}\", "
        "expr=\"{}\", pos=\"{}\"",
        mCharacter,
        mAlias,
        mExpression,
        mPosition
    );
    ctx.push();
    for(auto &&s : mStatements)
        s->print(ctx);
    ctx.pop();
}
}
