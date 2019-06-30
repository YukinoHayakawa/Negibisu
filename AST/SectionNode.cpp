#include "SectionNode.hpp"

#include "CommandNode.hpp"
#include "DialogNode.hpp"

namespace usagi::negi
{
void SectionNode::print(std::string &indentation)
{
    fmt::print(
        "{}SECTION: script_name=\"{}\", display_name=\"{}\"\n",
        indentation,
        mScriptName, mDisplayName
    );
    indentation.append(INDENTATION, ' ');
    for(auto &&line : mLines)
        line->print(indentation);
    indentation.erase(indentation.end() - INDENTATION, indentation.end());
}

void SectionNode::parseTitle()
{
    if(currentType() == TokenType::SHARP)
    {
        consume(TokenType::SHARP);
        mScriptName = consumeString();
        consume(TokenType::COLON);
        mDisplayName = consumeString();
    }
    else
    {
        mScriptName = mParsingContext->createToken(
            TokenType::STRING_LITERAL,
            "<UNNAMED>"
        );
        mDisplayName = mParsingContext->createToken(
            TokenType::STRING_LITERAL,
            "<UNNAMED SECTION>"
        );
    }
    mSceneContext.symbol_table.lookup(mScriptName.ref, SymbolType::SCRIPT);
}

void SectionNode::parseContent()
{
    while(streamNotEnded())
    {
        try
        {
            switch(currentType())
            {
                case TokenType::LEFT_BRACKET:
                case TokenType::STRING_LITERAL:
                    parseDialog();
                    continue;
                case TokenType::LEFT_BRACE:
                    parseCommand();
                    continue;
                // next section
                case TokenType::SHARP:
                    return;
                case TokenType::NEWLINE:
                    advance();
                    continue;
                default:
                    syntaxError("Expected a line of text or a command.");
            }
        }
        catch(const SyntaxError &)
        {
            proceedToNextLine();
        }
    }
}

void SectionNode::parseDialog()
{
    // exception safe
    auto line = std::make_unique<DialogNode>(mParsingContext);
    line->parse(&mSceneContext);
    mLines.push_back(std::move(line));
}

void SectionNode::parseCommand()
{
    auto line = std::make_unique<CommandNode>(mParsingContext);
    line->parse(&mSceneContext);
    mLines.push_back(std::move(line));
}

void SectionNode::parse(SceneContext *ctx)
{
    parseTitle();
    parseContent();
}

void SectionNode::check(SceneContext *ctx)
{
    for(auto &&l : mLines)
        l->check(&mSceneContext);
}

void SectionNode::generate(SceneContext *ctx)
{
    for(auto &&l : mLines)
        l->generate(&mSceneContext);
}
}
