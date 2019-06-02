#pragma once

#include <string>

#include <Usagi/Utility/Noncopyable.hpp>

namespace usagi::negibisu
{
class StringParser : Noncopyable
{
	// source code stream in utf-32 code points
	std::u32string mSource;
	// parsed characters converted back to utf-8 bytes, referenced by the
	// string views in tokens
	std::string mUtf8Source;

	// position tracking
	int mCurrentLine = 1;
	int mCurrentColumn = 1;
	// index of current char in the utf-32 stream
	std::size_t mCurrentPos = 0;

	std::size_t mSubstringBegin = std::string::npos;

protected:
	virtual ~StringParser() = default;

	static bool isSpaceChar(char32_t c);
	void skipWhiteSpace();

	// return the character pointed by current position
	char32_t cur() const;
	// return the value of later character without actually moving the cursor.
	char32_t peek(std::size_t lookahead = 1) const;
	// return the character on the position immediately before the current
	// position.
	char32_t last() const;

	//
	/**
	 * \brief Advance the cursor by one and return the character on the
	 * new position. If the stream is ended, a 0 will be returned.
	 * \param append If true, the current character before advancing will
	 * be appended to the parsed uft-8 string.
	 * \return
	 */
	char32_t advance(bool append = true);

	// report error with current source position.
	void error(const std::string &msg) const;

	void beginSubstring();
	/**
	 * \brief
	 * \param trim_back remove last n bytes in utf-8 size
	 * \return
	 */
	std::string_view endSubstring(std::size_t trim_back = 0);

	int currentLine() const
	{
		return mCurrentLine;
	}

	int currentColumn() const
	{
		return mCurrentColumn;
	}

	std::size_t currentUtf8Size() const
	{
		return mUtf8Source.size();
	}

	virtual void onNewLine();

public:
	/**
	 * \brief
	 * \param src Input stream of source code in utf-8 bytes
	 */
	explicit StringParser(std::istream &src);

	/**
	 * \brief
	 * \param src Input stream of source code in utf-8 bytes
	 */
	explicit StringParser(const std::string &src);
};


}
