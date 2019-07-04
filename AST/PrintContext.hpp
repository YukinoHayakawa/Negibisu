#pragma once

#include <ostream>

#include <fmt/ostream.h>

namespace usagi::negi
{
class PrintContext
{
    static constexpr int INDENTATION = 2;

    std::string mIndentation;

public:
    std::ostream *output = nullptr;

    void push();
    void pop();

    template <typename... Args>
    void print(Args &&... args) const
    {
        fmt::print(*output, mIndentation);
        fmt::print(*output, std::forward<Args>(args)...);
        fmt::print(*output, "\n");
    }
};
}
