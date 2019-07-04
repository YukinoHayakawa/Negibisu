#pragma once

#include <unordered_map>
#include <unordered_set>
#include <limits>

#include <Usagi/Utility/Noncopyable.hpp>

#include <Negibisu/Lexical/Token.hpp>

namespace usagi::negi
{
enum class SymbolType
{
    // Description of the character properties.
    CHARACTER,
    POSITION,
    // Image layers, can be background or cg images.
    IMAGE_LAYER,
    // Description of character expressions, including the positioning of
    // expression images.
    EXPRESSION,
    // One-time character voice.
    VOICE,
    // Short sound effects.
    SOUND_EFFECT,
    // Background musics.
    MUSIC,
    // Other scripts of scenes or story blocks.
    SCRIPT,
    // Scene description file, including character position definitions.
    SCENE,
    // Translatable game texts.
    GAME_TEXT,
    IDENTIFIER,
    UNKNOWN,
};

std::string_view to_string(SymbolType t);
std::ostream & operator<<(std::ostream &os, SymbolType t);

struct SymbolInfo
{
    SymbolType type = SymbolType::UNKNOWN;
    std::string object_name;
    // the position of the first usage of the referenced asset
    std::size_t first_usage = std::numeric_limits<std::size_t>::max();
    // the position of the last usage of the referenced asset
    std::size_t last_usage = 0;

    void extendScope(std::size_t position);
};

// https://en.wikipedia.org/wiki/Symbol_table
// https://www.tutorialspoint.com/compiler_design/compiler_design_symbol_table.htm
class SymbolTable : Noncopyable
{
    // SymbolNameCounter name_counter;
    std::size_t mResourceIndex = 0;

public:
    std::unordered_map<std::string_view, SymbolInfo> symbols;
    std::unordered_set<std::string_view> string_literals;

    // void insert(const std::string_view &name, SymbolType type);
    const SymbolInfo & lookup(const Token *token, SymbolType type);
    void addStringLiteral(const Token *token);

    void dumpSymbols(std::ostream &output) const;
    void dumpStringLiterals(std::ostream &output) const;
};
}
