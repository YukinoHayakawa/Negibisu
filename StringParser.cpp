﻿#include "StringParser.hpp"

#include <utfcpp/utf8.h>

#include <Usagi/Core/Logging.hpp>
#include <Usagi/Utility/Stream.hpp>
#include <Usagi/Utility/Debug.hpp>

namespace usagi::negibisu
{
bool StringParser::isSpaceChar(char32_t c)
{
	return c == ' '
		|| c == '\t'
		|| c == '\n'
		|| c == '\v'
		|| c == '\f'
		|| c == '\r';
}

void StringParser::skipWhiteSpace()
{
	// discard whitespace characters
	while(isSpaceChar(cur()))
		advance(false);
	assert(!isSpaceChar(cur()));
}

char32_t StringParser::cur() const
{
	return mSource[mCurrentPos];
}

char32_t StringParser::next(const std::size_t lookahead) const
{
	const auto next_pos = mCurrentPos + lookahead;
	return next_pos >= mSource.size() ? 0 : mSource[next_pos];
}

char32_t StringParser::prev() const
{
	return mCurrentPos > 0 ? mSource[mCurrentPos - 1] : 0;
}

void StringParser::advanceLineColumnCounter()
{
	// assuming unix end-line
	if(cur() == '\n')
	{
		++mCurrentLine;
		mCurrentColumn = 1;
	}
	else
	{
		++mCurrentColumn;
	}
}

void StringParser::advanceSourceCursor()
{
	++mCurrentPos;
}

void StringParser::advanceCursor()
{
	advanceLineColumnCounter();
	advanceSourceCursor();
}

char32_t StringParser::advance(const bool append, const bool allow_continue)
{
	assert(mCurrentPos < mSource.size());

	// check newline before next advance to avoid interrupting running strings
	if(cur() == '\n')
	{
		onNewLine();
	}

	if(append)
	{
		utf8::append(cur(), back_inserter(mUtf8Source));
	}

	// move to the next character
	advanceCursor();

	if(cur() == '\\')
	{
		// continue current line, move cursor to the beginning of next line
		if(allow_continue && next() == '\n')
		{
			advanceCursor();
			advanceCursor();
		}
	}
	else if(cur() == 7)
	{
		debugBreak();
		// skip debug mark
		advanceCursor();
	}

	return cur();
}

void StringParser::error(const std::string &msg) const
{
	LOG(error, "at Line {}, Col {}: {}", mCurrentLine, mCurrentColumn, msg);
}

StringParser::StringParser(std::istream &u8src)
	: StringParser(readStreamAsString(u8src))
{
}

StringParser::StringParser(const std::string &u8src)
{
	mUtf8Source.reserve(u8src.size());

	LOG(debug, "Source code length: {}", u8src.size());

	// convert to utf-32 for easier text manipulation, convert back
	// to utf-8 during parsing to know the positions of tokens
	// in the utf-8 stream. the codepoint stream might also be filtered
	// during processing (such as for escaping sequences)
	utf8::utf8to32(
		u8src.begin(), u8src.end(),
		back_inserter(mSource)
	);
}

void StringParser::beginSubstring()
{
	mSubstringBegin = mUtf8Source.size();
}

std::string_view StringParser::endSubstring(const std::size_t trim_back)
{
	assert(mSubstringBegin != std::string::npos);

	// if(trim_back)
	// 	mUtf8Source.erase(mUtf8Source.end() - trim_back, mUtf8Source.end());

	const std::string_view subview {
		mUtf8Source.data() + mSubstringBegin,
		mUtf8Source.size() - mSubstringBegin - trim_back
	};

	mSubstringBegin = std::string::npos;

	return subview;
}

void StringParser::onNewLine()
{
}
}
