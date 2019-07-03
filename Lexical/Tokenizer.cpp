#include "Tokenizer.hpp"

#include <fmt/printf.h>

namespace usagi::negi
{
void Tokenizer::tokenize()
{
    while(cur())
    {
        if(cur() == '\n')
            advance();
        if(currentEnvironment() != Environment::QUOTED_STRING)
            skipWhiteSpace();
        beginToken();
        const auto this_char = cur();
        // todo: preprocessor directives
        // if(this_char == '$' && lastTokenType() == TokenType::NEWLINE)
        // {
        //
        // }
        if(this_char == '#')
        {
            if(const auto last_type = lastTokenType();
                last_type == TokenType::NEWLINE
                || last_type == TokenType::SHARP)
            {
                advance();
                endToken(TokenType::SHARP);
                enterEnvironment(Environment::TITLE);
                continue;
            }
        }
        if(this_char == '{')
        {
            if(next() == '{')
            {
                ignoreComment();
            }
            else
            {
                advance();
                endToken(TokenType::LEFT_BRACE);
                enterEnvironment(Environment::COMMAND);
            }
            continue;
        }
        if(this_char == '}')
        {
            if(next() == '}')
            {
                // }} should already be consumed by ignoreComment()
                assert(false);
            }
            else
            {
                exitEnvironment(Environment::COMMAND);
                advance();
                endToken(TokenType::RIGHT_BRACE);
            }
            continue;
        }
        if(this_char == '[')
        {
            advance();
            endToken(TokenType::LEFT_BRACKET);
            enterEnvironment(Environment::CHARACTER);
            continue;
        }
        if(this_char == ']')
        {
            exitEnvironment(Environment::CHARACTER);
            advance();
            endToken(TokenType::RIGHT_BRACKET);
            continue;
        }
        if(this_char == '"')
        {
            advance();
            if(currentEnvironment() == Environment::QUOTED_STRING)
            {
                exitEnvironment(Environment::QUOTED_STRING);
            }
            else
            {
                enterEnvironment(Environment::QUOTED_STRING);
                beginToken();
                readStringLiteral();
            }
            continue;
        }
        if(currentEnvironment() == Environment::COMMAND
            || currentEnvironment() == Environment::CHARACTER)
        {
            if(this_char == ':')
            {
                advance();
                endToken(TokenType::COLON);
                continue;
            }
            if(this_char == '=')
            {
                advance();
                endToken(TokenType::EQUAL);
                continue;
            }
            if(this_char == ',')
            {
                advance();
                endToken(TokenType::COMMA);
                continue;
            }
        }
        if(currentEnvironment() == Environment::TITLE)
        {
            if(this_char == ':')
            {
                advance();
                endToken(TokenType::COLON);
                continue;
            }
        }
        readStringLiteral();
    }
}

void Tokenizer::resetEnvironment()
{
    mEnv = { Environment::GLOBAL };
}

void Tokenizer::onNewLine()
{
    // append newline token
    beginToken();
    endToken(TokenType::NEWLINE);

    if(const auto cur_env = currentEnvironment();
        cur_env != Environment::GLOBAL)
    {
        assert(mEnv.size() > 1);

        switch(cur_env)
        {
            case Environment::COMMAND:
            case Environment::CHARACTER:
            case Environment::COMMENT:
            case Environment::QUOTED_STRING:
                currentSourcePosition().error(
                    "Expected a {}.",
                    tokenSymbol(envCloseSymbol(cur_env))
                );
            default: ;
        }
        // reset env on newline
        resetEnvironment();
    }
}

TokenType Tokenizer::envOpenSymbol(const Environment env)
{
    switch(env)
    {
        case Environment::COMMAND: return TokenType::LEFT_BRACE;
        case Environment::CHARACTER: return TokenType::LEFT_BRACKET;
        case Environment::COMMENT: return TokenType::LEFT_DOUBLE_BRACE;
        case Environment::QUOTED_STRING:
            return TokenType::DOUBLE_QUOTE;
        default: return TokenType::UNKNOWN;
    }
}

TokenType Tokenizer::envCloseSymbol(const Environment env)
{
    switch(env)
    {
        case Environment::COMMAND: return TokenType::RIGHT_BRACE;
        case Environment::CHARACTER: return TokenType::RIGHT_BRACKET;
        case Environment::COMMENT: return TokenType::RIGHT_DOUBLE_BRACE;
        case Environment::QUOTED_STRING:
            return TokenType::DOUBLE_QUOTE;
        default: return TokenType::UNKNOWN;
    }
}

Tokenizer::Environment Tokenizer::currentEnvironment() const
{
    return mEnv.back();
}

void Tokenizer::enterEnvironment(Environment env)
{
    mEnv.push_back(env);
}

void Tokenizer::exitEnvironment(Environment env)
{
    assert(env != Environment::GLOBAL);
    const auto matched = env == currentEnvironment();

    if(matched)
    {
        assert(mEnv.size() > 1);
        mEnv.pop_back();
    }
    else
    {
        switch(env)
        {
            case Environment::COMMAND:
            case Environment::CHARACTER:
            case Environment::COMMENT:
                currentSourcePosition().error(
                    "Expected a matching {}.",
                    tokenSymbol(envOpenSymbol(env))
                );
                break;
            default:
                break;
        }
    }
}

void Tokenizer::beginToken()
{
    mTempToken.pos = currentSourcePosition();
    beginSubstring();
}

void Tokenizer::endToken(TokenType type, std::size_t trim_back)
{
    mTempToken.type = type;
    mTempToken.text = endSubstring(trim_back);
    mTempToken.index = mTokens.size();
    mTokens.push_back(mTempToken);
}

bool Tokenizer::isOperatorCharInCharacterName(char32_t c)
{
    return isEnvironmentBoundaryChar(c)
        || c == ','
        || c == '='
        || c == ':';
}

bool Tokenizer::isOperatorCharInCommand(char32_t c)
{
    return isEnvironmentBoundaryChar(c)
        || c == ','
        || c == ':';
}

bool Tokenizer::isOperatorCharInTitle(char32_t c)
{
    return isEnvironmentBoundaryChar(c)
        || c == ':';
}

bool Tokenizer::isOperatorCharInQuotedString(char32_t c)
{
    return isEnvironmentBoundaryChar(c);
}

bool Tokenizer::isEnvironmentBoundaryChar(char32_t c)
{
    return c == '['
        || c == ']'
        || c == '{'
        || c == '}'
        || c == '"';
}

void Tokenizer::readStringLiteral()
{
    const std::size_t begin_u8_size = currentUtf8Size();
    std::size_t last_non_space_size = begin_u8_size;
    bool escape_next = false;
    bool end_of_line = false;

    // if not in command block, read til next command begin token,
    // otherwise read til next control token
    while(const auto this_char = cur())
    {
        if(this_char == '\n')
        {
            end_of_line = true;
            break;
        }
        // escape next character. note that \\\n is hidden from by advance()
        if(this_char == '\\' && !escape_next)
        {
            advance(false);
            escape_next = true;
            goto read_next;
        }
        // always append the escaped character
        if(escape_next)
        {
            if(isSpaceChar(this_char))
            {
                skipWhiteSpace();
            }
            else
            {
                advance();
            }
            escape_next = false;
            goto read_next;
        }
        if(commentOpening())
        {
            ignoreComment();
            continue;
        }
        if(currentEnvironment() == Environment::GLOBAL)
        {
            if(isEnvironmentBoundaryChar(this_char))
                break;
        }
        else if(currentEnvironment() == Environment::COMMAND)
        {
            if(isOperatorCharInCommand(this_char))
                break;
        }
        else if(currentEnvironment() == Environment::CHARACTER)
        {
            if(isOperatorCharInCharacterName(this_char))
                break;
        }
        else if(currentEnvironment() == Environment::TITLE)
        {
            if(isOperatorCharInTitle(this_char))
                break;
        }
        else if(currentEnvironment() == Environment::QUOTED_STRING)
        {
            if(isOperatorCharInQuotedString(this_char))
                break;
        }
        // move onto next char
        advance();

read_next:
        if(!isSpaceChar(this_char))
            last_non_space_size = currentUtf8Size();
    }
    // trailing spaces are trimmed unless they are quoted.
    std::size_t trim_back_space = 0;
    if(currentEnvironment() != Environment::QUOTED_STRING)
        trim_back_space = currentUtf8Size() - last_non_space_size;
    const auto len = currentUtf8Size() - begin_u8_size - trim_back_space;
    if(len > 0)
    {
        endToken(TokenType::STRING_LITERAL, trim_back_space);
    }
}

TokenType Tokenizer::lastTokenType() const
{
    if(mTokens.empty())
        return TokenType::NEWLINE;
    return mTokens.back().type;
}

bool Tokenizer::commentOpening()
{
    return cur() == '{' && next() == '{';
}

bool Tokenizer::commentClosing()
{
    return cur() == '}' && next() == '}';
}

void Tokenizer::ignoreComment()
{
    assert(commentOpening());
    advance(false);
    advance(false);
    enterEnvironment(Environment::COMMENT);
    while(cur())
    {
        if(commentClosing())
        {
            exitEnvironment(Environment::COMMENT);
            advance(false);
            advance(false);
            return;
        }
        advance(false, false, true);
    }
    currentSourcePosition().error(
        "Expected a {}.",
        tokenSymbol(envCloseSymbol(Environment::COMMENT))
    );
}

void Tokenizer::dumpTokens()
{
    for(auto &&token : mTokens)
    {
        fmt::print("{}: [{}] {}\n",
            token.pos,
            tokenName(token.type),
            token.text
        );
    }
}
}
