#pragma once

#include <vector>

#include <Negibisu/AST/StatementNode.hpp>
#include <Negibisu/Lexical/Token.hpp>

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

class IntrinsicCommand : public StatementNode
{
public:
    virtual std::initializer_list<CommandParameterInfo> parameterInfo()
        const = 0;

    // implementations of derived classes must first call this version
    virtual void fillArguments(
        SceneContext *ctx,
        const std::vector<TokenRef> &args) = 0;

    void parse(ParsingContext *ctx) override final;

    // returns empty pointer if not found
    static std::unique_ptr<IntrinsicCommand> find(std::string_view name);
};
}
