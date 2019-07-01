#pragma once

#include <ostream>

#include <fmt/ostream.h>

namespace usagi::negi
{
class PrintContext
{
    static constexpr int INDENTATION = 2;

    std::ostream &mOutput;
    std::string mIndentation;

public:
    explicit PrintContext(std::ostream &output);

    void push();
    void pop();

    template <typename... Args>
    void print(Args &&... args) const
    {
        fmt::print(mOutput, mIndentation);
        fmt::print(mOutput, std::forward<Args>(args)...);
        fmt::print(mOutput, "\n");
    }
};
}
