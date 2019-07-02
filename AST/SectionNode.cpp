#include "SectionNode.hpp"

#include <Negibisu/Parsing/ParsingContext.hpp>

#include "PrintContext.hpp"
#include "Command/SystemCommand.hpp"
#include "Command/CharacterCommand.hpp"
#include "Statement/CommandExecutionNode.hpp"

namespace usagi::negi
{
void SectionNode::print(PrintContext &ctx) const
{
    ctx.print(
        "SECTION: script_name=\"{}\", display_name=\"{}\"",
        mScriptName, mDisplayName
    );
    ctx.push();
    for(auto &&s : mStatements)
        s->print(ctx);
    ctx.pop();
}

void SectionNode::parseTitle(ParsingContext *ctx)
{
    if(ctx->currentType() == TokenType::SHARP)
    {
        ctx->consume(TokenType::SHARP);
        mScriptName = ctx->consumeString();
        ctx->consume(TokenType::COLON);
        mDisplayName = ctx->consumeString();
    }
    else
    {
        mScriptName = ctx->createToken(
            TokenType::STRING_LITERAL,
            "<UNNAMED>"
        );
        mDisplayName = ctx->createToken(
            TokenType::STRING_LITERAL,
            "<UNNAMED SECTION>"
        );
    }
}

void SectionNode::parseLine(ParsingContext *ctx)
{
    ctx->line = LineContext { };

    while(ctx->streamNotEnded())
    {
        switch(ctx->currentType())
        {
            case TokenType::LEFT_BRACKET:
            {
                auto stat = std::make_unique<CharacterTag>();
                stat->parse(ctx);
                mStatements.push_back(std::move(stat));
                break;
            }
            case TokenType::STRING_LITERAL:
            {
                auto text = ctx->consumeString();
                if(ctx->line.current_character)
                {
                    auto stat = std::make_unique<CharacterSayCommand>(
                        ctx->line.current_character, text);
                    mStatements.push_back(std::move(stat));
                }
                else
                {
                    auto stat = std::make_unique<NarratorSayCommand>(
                        text);
                    mStatements.push_back(std::move(stat));
                }
                ctx->line.any_dialog = true;
                break;
            }
            case TokenType::LEFT_BRACE:
            {
                auto stat = std::make_unique<CommandExecutionNode>();
                stat->parse(ctx);
                mStatements.push_back(std::move(stat));
                break;
            }
            // next section
            case TokenType::SHARP:
            {
                return;
            }
            case TokenType::NEWLINE:
            {
                ctx->advance();
                if(!ctx->line.beginning)
                    mStatements.push_back(
                        std::make_unique<SystemWaitInputCommand>()
                    );
                return;
            }
            default:
            {
                ctx->syntaxError("Expected dialog text or a command.");
            }
        }
        ctx->line.beginning = false;
    }
}

void SectionNode::parseContent(ParsingContext *ctx)
{
    while(ctx->streamNotEnded())
    {
        // next section
        if(ctx->currentType() == TokenType::SHARP)
            return;
        try
        {
            parseLine(ctx);
        }
        catch(const SyntaxError &)
        {
            ctx->proceedToNextLine();
        }
    }
}

void SectionNode::parse(ParsingContext *ctx)
{
    parseTitle(ctx);
    parseContent(ctx);
}

void SectionNode::check(SceneContext *ctx)
{
    mSceneContext.symbol_table.lookup(mScriptName.ref, SymbolType::SCRIPT);

    std::size_t errors = 0;

    for(auto &&l : mStatements)
    {
        try
        {
            l->check(&mSceneContext);
        }
        catch(const SemanticError &)
        {
            ++errors;
        }
    }
    if(errors)
        mSceneContext.print("{} semantic errors.", errors);

    mChecked = errors == 0;
}

void SectionNode::generate(SceneContext *ctx) const
{
    if(!mChecked)
    {
        mSceneContext.print(
            "Source program isn't valid. No code was generated."
        );
    }
    else
    {
        for(auto &&l : mStatements)
            l->generate(&mSceneContext);
    }
}
}
