#pragma once

#include <vector>
#include <list>

#include <Negibisu/Lexical/Token.hpp>

namespace usagi::negi
{
class SyntaxError : public std::runtime_error
{
public:
    SyntaxError()
        : runtime_error("Syntax error.")
    {
    }
};

struct LineContext
{
    bool beginning = true;
    bool any_dialog = false;
    TokenRef current_character;
};

struct ParsingContext
{
    const std::vector<Token> &tokens;
    // modifying token vector will invalidate the pointers, therefore put
    // extra tokens in a list.
    std::list<Token> extra_tokens;

    using TokenStreamIterator = std::vector<Token>::const_iterator;

    TokenStreamIterator cursor;
    const TokenStreamIterator end;

    TokenType currentType() const;
    TokenType peekType(std::size_t lookahead = 1) const;
    const Token * peek(std::size_t lookahead = 1) const;
    const Token & cur() const;
    TokenRef curRef() const;

    void advance();
    void skipNewLines();
    bool streamEnded() const;
    bool streamNotEnded() const;

    void expect(TokenType token_type) const;
    void consume(TokenType token_type);
    TokenRef consumeString();

    void proceedToNextLine();

    explicit ParsingContext(const std::vector<Token> &tokens);

    TokenRef createToken(TokenType type, std::string_view text);

    template <typename... Args>
    void error(Args &&... args) const
    {
        cursor->pos.error(std::forward<Args>(args)...);
    }

    template <typename... Args>
    void syntaxError(Args &&... args) const
    {
        error(std::forward<Args>(args)...);
        USAGI_THROW(SyntaxError());
    }

    LineContext line;
};
}
