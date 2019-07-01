#include "PrintContext.hpp"

namespace usagi::negi
{
PrintContext::PrintContext(std::ostream &output)
    : mOutput(output)
{
}

void PrintContext::push()
{
    mIndentation.append(INDENTATION, ' ');
}

void PrintContext::pop()
{
    mIndentation.erase(mIndentation.end() - INDENTATION, mIndentation.end());
}
}
