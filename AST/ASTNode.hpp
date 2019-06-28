#pragma once

#include <vector>

#include <Usagi/Utility/Noncopyable.hpp>

#include "../Lexical/Token.hpp"

namespace usagi::negi
{
struct SymbolTable;

class ASTNode : Noncopyable
{
    template <typename... Args>
    void error(Args &&... args) const
    {
        mTokenBegin->pos.error(std::forward<Args>(args)...);
    }

protected:
    static constexpr int INDENTATION = 2;

    using TokenStreamIterator = std::vector<Token>::const_iterator;

    TokenStreamIterator &mTokenBegin;
    const TokenStreamIterator &mTokenEnd;

    TokenType currentType() const;
    TokenType nextType(std::size_t lookahead = 1) const;
    const Token & cur() const;
    TokenRef curRef() const;

    void advance();
    void skipNewLines();
    bool streamEnded() const;
    bool streamNotEnded() const;

    void expect(TokenType token_type) const;
    void consume(TokenType token_type);
    TokenRef consumeString();

    template <typename... Args>
    void syntaxError(Args &&... args) const
    {
        error(std::forward<Args>(args)...);
        throw std::runtime_error("Syntax error.");
    }

public:
    ASTNode(
        TokenStreamIterator &token_begin,
        const TokenStreamIterator &token_end);
    virtual ~ASTNode() = default;
    ASTNode(ASTNode &&) = default;

    virtual void parse(SymbolTable *table) = 0;
    virtual void print(std::string &indentation) = 0;
    virtual void check(SymbolTable *table) { }
};
}
