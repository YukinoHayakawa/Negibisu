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
        // case SymbolType::VOICE: return "Voice";
        // case SymbolType::SOUND_EFFECT: return "SoundEffect";
        // case SymbolType::MUSIC: return "Music";
        case SymbolType::SCRIPT: return "Script";
        case SymbolType::SCENE: return "Scene";
        case SymbolType::POSITION: return "Position";
        case SymbolType::GAME_TEXT: return "GameText";
        case SymbolType::IDENTIFIER: return "Identifier";
        case SymbolType::UNKNOWN: return "Unknown";
        case SymbolType::AUDIO_TRACK: return "AudioTrack";
        default: return "Unknown";
    }
}

std::string_view to_string(AssetType t)
{
    switch(t)
    {
        case AssetType::IMAGE: return "IMAGE";
        case AssetType::CHARACTER: return "CHARACTER";
        case AssetType::SCENE: return "SCENE";
        case AssetType::SCRIPT: return "SCRIPT";
        case AssetType::EXPRESSION: return "EXPRESSION";
        case AssetType::AUDIO: return "AUDIO";
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

const SymbolInfo & SymbolTable::lookup(
    const Token *token,
    const SymbolType type)
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
        symbol.object_name = fmt::format(
            "res_{:08x}",
            static_cast<std::uint32_t>(
                std::hash<std::string_view>()(token->text)
            )
        );
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

    return symbol;
}

void SymbolTable::addStringLiteral(const Token *token)
{
    string_literals.emplace(token->text);
}

void SymbolTable::addAssetRef(AssetType type, const Token *token)
{
    asset_refs.insert({ type, token->text });
}

void SymbolTable::dumpSymbols(std::ostream &output) const
{
    for(auto &&s : symbols)
    {
        fmt::print(output, "{}: {}\n", s.first, s.second.type);
    }
}

void SymbolTable::dumpStringLiterals(std::ostream &output) const
{
    for(auto &&s : string_literals)
    {
        fmt::print(output, "\"{}\"\n", s);
    }
}

void SymbolTable::dumpAssetRefs(std::ostream &output) const
{
    for(auto &&a : asset_refs)
    {
        fmt::print(output, "{}\n{}\n", to_string(a.first), a.second);
    }
}
}
