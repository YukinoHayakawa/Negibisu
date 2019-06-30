#pragma once

#include <string>

namespace usagi::negi
{
class SymbolNameCounter
{
    std::string mName = "a";

public:
    void increment();
    std::string current() const;

    SymbolNameCounter & operator++()
    {
        increment();
        return *this;
    }
};
}
