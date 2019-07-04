#include "PrintContext.hpp"

namespace usagi::negi
{
void PrintContext::push()
{
    mIndentation.append(INDENTATION, ' ');
}

void PrintContext::pop()
{
    mIndentation.erase(mIndentation.end() - INDENTATION, mIndentation.end());
}
}
