#include "SourcePosition.hpp"

namespace usagi::negibisu
{
std::ostream & operator<<(std::ostream &os, const SourcePosition &p)
{
    fmt::print(os, "Line {}, Col {}", p.line, p.column);
    return os;
}
}
