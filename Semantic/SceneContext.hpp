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
    std::string_view alias;
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

    std::ostream *output = nullptr;

    template <typename... Args>
    void print(Args &&... args) const
    {
        fmt::print(*output, std::forward<Args>(args)...);
        fmt::print(*output, "\n");
    }

    template <typename... Args>
    void semanticError(TokenRef position_hint, Args &&... args) const
    {
        position_hint->pos.error(std::forward<Args>(args)...);
        USAGI_THROW(SemanticError());
    }

    template <typename... Args>
    void semanticWarning(TokenRef position_hint, Args &&... args) const
    {
        position_hint->pos.warning(std::forward<Args>(args)...);
    }

    CharacterState & characterState(TokenRef character);
    CharacterState & ensureCharacterOnStage(TokenRef character);
};

}
