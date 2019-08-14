#pragma once

#include <iostream>

#include <Usagi/Core/Format.hpp>

namespace usagi::negi
{
struct SourcePosition
{
    int line = 1, column = 1;

    template <typename... Args>
    void error(Args &&... args) const
    {
        fmt::print(std::cerr, "Error at {}: {}\n",
            *this,
            fmt::format(std::forward<Args>(args)...)
        );
    }

    template <typename... Args>
    void warning(Args &&... args) const
    {
        fmt::print(std::cerr, "Warning at {}: {}\n",
            *this,
            fmt::format(std::forward<Args>(args)...)
        );
    }
};

std::ostream &operator<<(std::ostream &os, const SourcePosition &p);
}
