#include "CommandNode.hpp"

#include <Negibisu/Semantic/SceneContext.hpp>

namespace usagi::negi
{
void CommandNode::parseArgs()
{
    while(streamNotEnded())
    {
        mArgs.push_back(consumeString());
        switch(currentType())
        {
            case TokenType::COMMA:
                advance();
                continue;

            case TokenType::RIGHT_BRACE:
                return;

            default:
                syntaxError(
                    "Expected a {} or {}.",
                    tokenSymbol(TokenType::COMMA),
                    tokenSymbol(TokenType::RIGHT_BRACE)
                );
        }
    }
}

CommandNode::CommandNode(
    ParsingContext *parsing_context,
    std::vector<TokenRef> token_refs)
    : LineNode(parsing_context)
    , mArgs(std::move(token_refs))
{
}

void CommandNode::parse(SceneContext *ctx)
{
    consume(TokenType::LEFT_BRACE);
    mCommandName = consumeString();
    if(currentType() == TokenType::COLON)
    {
        advance();
        parseArgs();
    }
    consume(TokenType::RIGHT_BRACE);
}

void CommandNode::print(std::string &indentation)
{
    fmt::print("{}COMMAND: name=\"{}\", args=[{}]\n",
        indentation,
        mCommandName,
        fmt::join(mArgs.begin(), mArgs.end(), ", ")
    );
}

void CommandNode::check(SceneContext *ctx)
{
    const auto param_info = parameterInfo();

    // check argument number
    if(mArgs.size() < param_info.size())
    {
        ctx->semanticError(
            mArgs.size() == 0 ? mCommandName : mArgs.back(),
            "Too few arguments for command {}: expected {}, actually {}",
            mCommandName,
            param_info.size(),
            mArgs.size()
        );
    }
    if(mArgs.size() > param_info.size())
    {
        ctx->semanticError(
            mArgs.back(),
            "Too many arguments for command {}: expected {}, actually {}",
            mCommandName,
            param_info.size(),
            mArgs.size()
        );
    }

    // check argument types
    for(std::size_t i = 0; i < param_info.size(); ++i)
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
                throw std::logic_error("");
        }
    }
}
}
