#pragma once

#include <iostream>

#include <Usagi/Utility/Noncopyable.hpp>

#include "SymbolTable.hpp"

namespace usagi::negi
{
struct CharacterState
{
    std::string_view object_name;
    std::string_view current_expression;
    std::string_view current_position;
    std::string_view disguised_name;
    bool on_stage = false;
};

class SemanticError : public std::runtime_error
{
public:
    SemanticError()
        : runtime_error("Semantic error.")
    {
    }
};

struct SceneContext : Noncopyable
{
    SymbolTable symbol_table;
    std::unordered_map<std::string_view, CharacterState> characters;

    // todo fix this
    std::ostream *output = &std::cout;

    template <typename... Args>
    void print(Args &&... args) const
    {
        fmt::print(*output, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void semanticError(TokenRef position_hint, Args &&... args) const
    {
        position_hint->pos.error(std::forward<Args>(args)...);
        throw SemanticError();
    }

    CharacterState & characterState(TokenRef character);
    CharacterState & ensureCharacterOnStage(TokenRef character);
};

}
