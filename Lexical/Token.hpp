#pragma once

#include <string_view>
#include <charconv>

namespace usagi::negibisu
{
enum class TokenType
{
	END_OF_STREAM = 0,
	SHARP,
	NEWLINE,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_DOUBLE_BRACE,
    RIGHT_DOUBLE_BRACE,
	STRING_LITERAL,
	COLON,
	COMMA,
    EQUAL,
};

const char * tokenName(TokenType token);
const char * tokenSymbol(TokenType token);

struct Token
{
    int line = 0, column = 0;
    TokenType type = TokenType::END_OF_STREAM;
    std::string_view text;

    template <typename T>
    auto as()
    {
		T result;
		// https://en.cppreference.com/w/cpp/utility/from_chars
		if(auto [p, ec] = std::from_chars(
			text.data(), text.data() + text.size(), result);
			ec == std::errc())
			return result;
		throw std::bad_cast();
    }
};
}
