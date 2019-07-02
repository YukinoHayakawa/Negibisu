#pragma once

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

class IntrinsicCommand;

struct CommandParameterInfo
{
    std::string_view name;
    CommandParameterType type;
    TokenRef IntrinsicCommand::* value;

    CommandParameterInfo(
        std::string_view name,
        CommandParameterType type,
        TokenRef IntrinsicCommand::*value)
        : name(std::move(name))
        , type(type)
        , value(value)
    {
    }
};

using ParameterList = std::initializer_list<CommandParameterInfo>;

#define NEGI_RETURN_PARAMS(...) \
    static ParameterList params = { __VA_ARGS__ }; \
    return params \
/**/

#define NEGI_PARAM(name, type, member) { \
    name, CommandParameterType::type, \
    static_cast<TokenRef IntrinsicCommand::*>(\
        &std::remove_pointer_t<decltype(this)>::member\
    )} \
/**/

class IntrinsicCommand : public StatementNode
{
public:
    virtual ParameterList parameterInfo() const = 0;

    void parse(ParsingContext *ctx) override final;

    // returns empty pointer if not found
    static std::unique_ptr<IntrinsicCommand> find(std::string_view name);
};
}
