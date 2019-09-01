#include "SceneContext.hpp"

namespace usagi::negi
{
CharacterState & SceneContext::characterState(TokenRef character)
{
    auto && ref = characters[character->text];
    ref.last_ref = character;
    return ref;
}

CharacterState & SceneContext::ensureCharacterOnStage(TokenRef character)
{
    auto &state = characterState(character);

    if(!state.on_stage)
    {
        semanticError(character,
            "Character {} not on the stage.",
            character
        );
    }

    return state;
}
}
