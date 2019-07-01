#pragma once

#include <vector>

#include <Negibisu/AST/StatementNode.hpp>
#include <Negibisu/Lexical/Token.hpp>

namespace usagi::negi
{
class IntrinsicCommand;

class CommandExecutionNode : public StatementNode
{
    void parseArgs(ParsingContext *ctx);

protected:
    TokenRef mCommandName;
    // todo for memory efficiency, do not use vector
    std::vector<TokenRef> mArgs;
    std::unique_ptr<IntrinsicCommand> mInvocation;

public:
    void parse(ParsingContext *ctx) override final;
    void check(SceneContext *ctx) override;
    void generate(SceneContext *ctx) override;
    void print(PrintContext &ctx) override;
};
}
