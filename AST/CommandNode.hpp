#pragma once

#include "LineNode.hpp"

namespace usagi::negi
{
enum class CommandParameterType
{
    VOID = 0,
    STRING,
    INT,
    FLOAT,
};

struct CommandParameterInfo
{
    std::string_view name;
    CommandParameterType type;
};

class CommandNode : public LineNode
{
    void parseArgs();

protected:
    TokenRef mCommandName;
    // todo do not use vector for memory efficiency
    std::vector<TokenRef> mArgs;

public:
    using LineNode::LineNode;
    CommandNode(
        ParsingContext *parsing_context,
        std::vector<TokenRef> token_refs);

    void parse(SceneContext *ctx) override;
    void print(std::string &indentation) override;

    virtual std::initializer_list<CommandParameterInfo> parameterInfo()
        const = 0;
    void check(SceneContext *ctx) override = 0;
    void generate(SceneContext *ctx) override = 0;
};
}
