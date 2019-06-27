#pragma once

#include <string>

#include <Usagi/Utility/Noncopyable.hpp>

#include "SourcePosition.hpp"

namespace usagi::negibisu
{
class StringParser : Noncopyable
{
	std::string mName;

	// source code stream in utf-32 code points
	std::u32string mSource;
	// parsed characters converted back to utf-8 bytes, referenced by the
	// string views in tokens
	std::string mUtf8Source;

	// position tracking
    SourcePosition mCurrentSourcePosition;
	// index of current char in the utf-32 stream
	std::size_t mCharPos = 0;

	std::size_t mSubstringBegin = std::string::npos;

	void advanceLineColumnCounter();
	void advanceSourceCursor();
	void advanceCursor();

protected:
	virtual ~StringParser() = default;

	static bool isSpaceChar(char32_t c);
	void skipWhiteSpace();

	// return the character pointed by current position
	char32_t cur() const;
	// return the value of later character without actually moving the cursor.
	char32_t next(std::size_t lookahead = 1) const;
	// return the character on the position immediately before the current
	// position.
	char32_t prev() const;


    const SourcePosition & currentSourcePosition() const
    {
        return mCurrentSourcePosition;
    }

	/**
	 * \brief Advance the cursor by one and return the character on the
	 * new position. If the stream is ended, a 0 will be returned.
	 * \param append If true, the current character before advancing will
	 * be appended to the parsed uft-8 string.
	 * \param allow_continue If true, a slash (\) at the end of line will
	 * cause appending of the content of next line to the current one.
	 * \param ignore_newline
	 * \return
	 */
	char32_t advance(
		bool append = true,
		bool allow_continue = true,
		bool ignore_newline = false);

	void beginSubstring();
	/**
	 * \brief
	 * \param trim_back remove last n bytes in utf-8 size
	 * \return
	 */
	std::string_view endSubstring(std::size_t trim_back = 0);

	std::size_t currentUtf8Size() const
	{
		return mUtf8Source.size();
	}

	// invoked when a \n is encountered
	virtual void onNewLine();

public:
	/**
	 * \brief
	 * \param name
	 * \param src Input stream of source code in utf-8 bytes
	 */
	StringParser(const std::string &name, std::istream &src);

	/**
	 * \brief
	 * \param name
	 * \param src Input stream of source code in utf-8 bytes
	 */
	StringParser(const std::string &name, const std::string &src);
};


}
