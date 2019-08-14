#include "CommandExecutionNode.hpp"

#include <Negibisu/Parsing/ParsingContext.hpp>
#include <Negibisu/Semantic/SceneContext.hpp>
#include <Negibisu/AST/PrintContext.hpp>

#include "IntrinsicCommand.hpp"

namespace usagi::negi
{
void CommandExecutionNode::parseArgs(ParsingContext *ctx)
{
    while(ctx->streamNotEnded())
    {
        mArgs.push_back(ctx->consumeString());
        switch(ctx->currentType())
        {
            case TokenType::COMMA:
                ctx->advance();
                continue;

            case TokenType::RIGHT_BRACE:
                return;

            default:
                ctx->syntaxError(
                    "Expected a {} or {}.",
                    tokenSymbol(TokenType::COMMA),
                    tokenSymbol(TokenType::RIGHT_BRACE)
                );
        }
    }
}

void CommandExecutionNode::parse(ParsingContext *ctx)
{
    ctx->consume(TokenType::LEFT_BRACE);
    ctx->expect(TokenType::STRING_LITERAL);
    mCommandName = ctx->consumeString();
    if(ctx->currentType() == TokenType::COLON)
    {
        ctx->advance();
        parseArgs(ctx);
    }
    ctx->consume(TokenType::RIGHT_BRACE);
}

void CommandExecutionNode::check(SceneContext *ctx)
{
    mInvocation = IntrinsicCommand::find(mCommandName->text);

    if(!mInvocation)
    {
        ctx->semanticWarning(mCommandName,
            "Undefined command: \"{}\". No command will be generated.",
            mCommandName);
        return;
    }

    const auto param_info = mInvocation->parameterInfo();

    // check argument number
    if(mArgs.size() < param_info.size())
    {
        ctx->semanticError(
            mArgs.size() == 0 ? mCommandName : mArgs.back(),
            "Too few arguments for command {}: expected {}, actual {}",
            mCommandName,
            param_info.size(),
            mArgs.size()
        );
    }
    if(mArgs.size() > param_info.size())
    {
        ctx->semanticError(
            mArgs.back(),
            "Too many arguments for command {}: expected {}, actual {}",
            mCommandName,
            param_info.size(),
            mArgs.size()
        );
    }

    // check argument types and assign values
    std::size_t i = 0;
    for(auto &&p : param_info)
    {
        switch((param_info.begin() + i)->type)
        {
            case CommandParameterType::STRING:
                break;
            case CommandParameterType::INT:
                if(!mArgs[i]->convertibleTo<int>())
                {
                    ctx->semanticError(mArgs[i], "Expected an Int.");
                }
                break;
            case CommandParameterType::FLOAT:
                if(!mArgs[i]->convertibleTo<float>())
                {
                    ctx->semanticError(mArgs[i], "Expected an Float.");
                }
                break;
            default:
                USAGI_THROW(std::logic_error("Unexpected command parameter type."));
        }
        mInvocation.get()->*p.value = mArgs[i];
        ++i;
    }
    mInvocation->check(ctx);
}

void CommandExecutionNode::generate(SceneContext *ctx) const
{
    if(mInvocation)
        mInvocation->generate(ctx);
}

void CommandExecutionNode::print(PrintContext &ctx) const
{
    ctx.print("COMMAND_EXECUTION: name=\"{}\", args=[{}]",
        mCommandName,
        fmt::join(mArgs.begin(), mArgs.end(), ", ")
    );
    if(mInvocation)
    {
        ctx.push();
        mInvocation->print(ctx);
        ctx.pop();
    }
}
}
