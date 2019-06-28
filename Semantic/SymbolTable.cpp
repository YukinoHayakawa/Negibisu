#include "SymbolTable.hpp"

namespace usagi::negi
{
std::string_view to_string(SymbolType t)
{
    switch(t)
    {
        case SymbolType::CHARACTER: return "Character";
        case SymbolType::IMAGE_LAYER: return "ImageLayer";
        case SymbolType::EXPRESSION: return "Expression";
        case SymbolType::VOICE: return "Voice";
        case SymbolType::SOUND_EFFECT: return "SoundEffect";
        case SymbolType::MUSIC: return "Music";
        case SymbolType::SCRIPT: return "Script";
        case SymbolType::SCENE: return "Scene";
        case SymbolType::POSITION: return "Position";
        case SymbolType::GAME_TEXT: return "GameText";
        case SymbolType::IDENTIFIER: return "Identifier";
        case SymbolType::UNKNOWN: return "Unknown";
        default: return "Unknown";
    }
}

std::ostream & operator<<(std::ostream &os, SymbolType t)
{
    return os << to_string(t);
}

void SymbolInfo::extendScope(std::size_t position)
{
    assert(position != -1);

    first_usage = std::min(position, first_usage);
    last_usage = std::max(position, last_usage);
}

void SymbolTable::lookup(const Token *token, const SymbolType type)
{
    const auto ref = symbols.try_emplace(
        token->text,
        SymbolInfo { }
    );
    auto &symbol = ref.first->second;

    // first usage
    if(ref.second)
    {
        symbol.type = type;
    }
    else
    {
        if(symbol.type != type)
        {
            token->pos.error(
                "Symbol \"{}\": redefining {} as {}",
                token->text,
                symbol.type,
                type
            );
        }
        symbol.extendScope(token->index);
    }
}

void SymbolTable::addStringLiteral(const Token *token)
{
    string_literals.emplace(token->text);
}

void SymbolTable::dumpSymbols() const
{
    for(auto &&s : symbols)
    {
        fmt::print("{}: {}\n", s.first, s.second.type);
    }
}

void SymbolTable::dumpStringLiterals() const
{
    for(auto &&s : string_literals)
    {
        fmt::print("\"{}\"\n", s);
    }
}
}
