#pragma once

#include <Usagi/Utility/Noncopyable.hpp>

namespace usagi::negi
{
class PrintContext;
struct ParsingContext;
struct SceneContext;

class ASTNode : Noncopyable
{
public:
    ASTNode() = default;
    virtual ~ASTNode() = default;
    ASTNode(ASTNode &&) = default;

    virtual void parse(ParsingContext *ctx) = 0;
    virtual void check(SceneContext *ctx) = 0;
    virtual void generate(SceneContext *ctx) = 0;
    virtual void print(PrintContext &ctx) = 0;
};
}
