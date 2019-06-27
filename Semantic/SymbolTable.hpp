#pragma once

#include <unordered_map>
#include <limits>

#include <Usagi/Utility/Noncopyable.hpp>

#include "../Lexical/Token.hpp"

namespace usagi::negibisu
{
enum class SymbolType
{
	// Description of the character properties.
	CHARACTER,
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
    TEXT,
	UNKNOWN,
};

std::string_view to_string(SymbolType t);
std::ostream &operator<<(std::ostream &os, SymbolType t);

struct SymbolReferenceScope
{
	SymbolType type = SymbolType::UNKNOWN;
	// the position of the first usage of the referenced asset
	std::size_t first_usage = std::numeric_limits<std::size_t>::max();
    // the position of the last usage of the referenced asset
    std::size_t last_usage = 0;

    void extendScope(std::size_t position);
};

// https://en.wikipedia.org/wiki/Symbol_table
// https://www.tutorialspoint.com/compiler_design/compiler_design_symbol_table.htm
struct SymbolTable : Noncopyable
{
	std::unordered_map<std::string_view, SymbolReferenceScope> symbols;

	// void insert(const std::string_view &name, SymbolType type);
	void lookup(const Token *token, SymbolType type);
};
}
