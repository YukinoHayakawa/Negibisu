#pragma once

#include <Usagi/Utility/Noncopyable.hpp>

#include <Negibisu/Parsing/ParsingContext.hpp>

namespace usagi::negi
{
struct SceneContext;

class SyntaxError : public std::runtime_error
{
public:
    SyntaxError()
        : runtime_error("Syntax error.")
    {
    }
};

class ASTNode : Noncopyable
{
    template <typename... Args>
    void error(Args &&... args) const
    {
        mParsingContext->cursor->pos.error(std::forward<Args>(args)...);
    }

protected:
    static constexpr int INDENTATION = 2;

    ParsingContext *mParsingContext = nullptr;

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

    void proceedToNextLine();

    template <typename... Args>
    void syntaxError(Args &&... args) const
    {
        error(std::forward<Args>(args)...);
        throw SyntaxError();
    }

public:
    explicit ASTNode(ParsingContext *context);

    virtual ~ASTNode() = default;
    ASTNode(ASTNode &&) = default;

    virtual void parse(SceneContext *ctx) = 0;
    virtual void check(SceneContext *ctx) = 0;
    virtual void generate(SceneContext *ctx) = 0;

    virtual void print(std::string &indentation) = 0;
};
}
