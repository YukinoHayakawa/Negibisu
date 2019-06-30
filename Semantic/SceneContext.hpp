#pragma once

#include <Usagi/Utility/Noncopyable.hpp>

#include "SymbolTable.hpp"

namespace usagi::negi
{
struct CharacterState
{
    std::string_view object_name;
    std::string_view current_expression;
    std::string_view current_position;
    bool on_stage = false;

    void enterStage(TokenRef expression, TokenRef position)
    {
        if(on_stage) { }
    }

    void ensureOnStage(TokenRef expression, TokenRef position)
    {
    }

    void changeExpression(TokenRef expression)
    {
    }

    void changePosition(TokenRef expression)
    {
    }

    void exitStage()
    {
    }
};

struct SceneContext : Noncopyable
{
    SymbolTable symbol_table;
    std::unordered_map<std::string_view, CharacterState> characters;
    std::ostream *output = nullptr;
};

}
