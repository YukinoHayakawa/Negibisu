#include "Tokenizer.hpp"

#include <utfcpp/utf8.h>

#include <Usagi/Core/Logging.hpp>
#include <Usagi/Utility/Stream.hpp>

namespace usagi::negibisu
{
void Tokenizer::beginToken()
{
    mTempToken.line = mCurrentLine;
    mTempToken.column = mCurrentColumn;
    mTempPos = mCurrentPos;
}

void Tokenizer::endToken(TokenType token, std::size_t length)
{
    const auto cur = mU8Source.size();
    utf8::utf32to8(
        mSource.data() + mTempPos, mSource.data() + mTempPos + length,
        std::back_inserter(mU8Source)
    );
    mTempToken.type = token;
    mTempToken.text = {
        mU8Source.data() + cur,
        mU8Source.size() - cur
    };
    mTokens.push_back(mTempToken);
}

const char32_t & Tokenizer::cur() const { return mSource[mCurrentPos]; }

const char32_t & Tokenizer::advance()
{
    assert(mCurrentPos < mSource.size());

    // assuming unix end-line
    if(cur() == '\n')
    {
        beginToken();
        endToken(TokenType::NEWLINE, 0);

        ++mCurrentLine;
        mCurrentColumn = 1;
    }
    else { ++mCurrentColumn; }

    ++mCurrentPos;
    // if(mCurrentPos == mSource.size())
    //     LOG(info, "Toknenizer: Reached end-of-file");

    return cur();
}

char32_t Tokenizer::peek() const
{
    if(mCurrentPos + 1 >= mSource.size())
        return 0;
    return mSource[mCurrentPos + 1];
}

bool Tokenizer::isTokenChar(char32_t c)
{
    return c == '['
        || c == ']'
        || isCommandTokenChar(c)
        // these are only effective within [] and {} pairs
        || c == ','
        || c == '='
        || c == ':';
}

bool Tokenizer::isCommandTokenChar(char32_t c)
{
    return c == '{'
        || c == '}';
}

bool Tokenizer::isSpaceChar(char32_t c)
{
    return c == ' '
        || c == '\t'
        || c == '\n'
        || c == '\v'
        || c == '\f'
        || c == '\r';
}

void Tokenizer::skipWhiteSpace() { while(isSpaceChar(cur())) advance(); }

void Tokenizer::readStringLiteral()
{
    skipWhiteSpace();
    beginToken();
    std::size_t trim_end = 0;
    bool read_next = true;
    // if not in command block, read til end of line, otherwise read til
    // next control token
    while(peek() != '\n')
    {
        if(peek() == 0)
        {
            read_next = false;
            break;
        }
        // next is a token char, the last char is the end of string literal
        if(mPlainText) { if(!mComment && isCommandTokenChar(peek())) break; }
        else { if(isTokenChar(peek())) break; }
        // move onto next char
        advance();
        if(mComment && cur() == '}' && peek() == '}')
        {
            read_next = false;
            break;
        }

        if(isSpaceChar(cur()))
            ++trim_end;
        else
            trim_end = 0;
    }
    const auto len = mCurrentPos - mTempPos - trim_end + read_next;
    if(len > 0) { endToken(TokenType::STRING_LITERAL, len); }
    if(read_next) advance();
}

void Tokenizer::error()
{
    LOG(error, "at Line {}, Col {}", mCurrentLine, mCurrentColumn);
    exit(1);
}

void Tokenizer::dumpTokens()
{
    for(auto &&token : mTokens)
    {
        fmt::print("Line {}, Col {}: [{}] {}\n",
            token.line, token.column,
            tokenName(token.type),
            token.text
        );
    }
}

void Tokenizer::tokenize(std::istream &in)
{
    const auto u8src = usagi::readStreamAsString(in);
    mU8Source.reserve(u8src.size());
    // utf32 is easier to handle, although the two conversions would introduce
    // some extra work
    utf8::utf8to32(
        u8src.begin(), u8src.end(),
        back_inserter(mSource)
    );

    while(mCurrentPos != mSource.size())
    {
        skipWhiteSpace();
        beginToken();
        if(cur() == '[')
        {
            endToken(TokenType::LEFT_BRACKET, 1);
            mPlainText = false;
            advance();
            continue;
        }
        if(cur() == ']')
        {
            endToken(TokenType::RIGHT_BRACKET, 1);
            mPlainText = true;
            advance();
            continue;
        }
        if(cur() == '{')
        {
            if(peek() == '{')
            {
                advance();
                endToken(TokenType::LEFT_DOUBLE_BRACE, 2);
                mComment = true;
                advance();
                readStringLiteral();
            }
            else
            {
                endToken(TokenType::LEFT_BRACE, 1);
                mPlainText = false;
                advance();
            }
            continue;
        }
        if(cur() == '}')
        {
            if(peek() == '}')
            {
                advance();
                endToken(TokenType::RIGHT_DOUBLE_BRACE, 2);
                mComment = false;
            }
            else
            {
                endToken(TokenType::RIGHT_BRACE, 1);
                mPlainText = true;
            }
            advance();
            continue;
        }
        if(!mPlainText)
        {
            if(cur() == ':')
            {
                endToken(TokenType::COLON, 1);
                advance();
                continue;
            }
            if(cur() == '=')
            {
                endToken(TokenType::EQUAL, 1);
                advance();
                continue;
            }
            if(cur() == ',')
            {
                endToken(TokenType::COMMA, 1);
                advance();
                continue;
            }
        }
        readStringLiteral();
    }

    if(mTokens.empty())
    {
        LOG(warn, "Tokenizer: Empty file.");
    }
}
}
