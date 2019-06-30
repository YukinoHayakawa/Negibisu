#include "SymbolNameCounter.hpp"

namespace usagi::negi
{
void SymbolNameCounter::increment()
{
    ++mName[0];

    for(std::size_t i = 0; i < mName.size(); ++i)
    {
        if(mName[i] == 'z' + 1)
        {
            if(i + 1 == mName.size())
            {
                mName.push_back('a');
            }
            else
            {
                ++mName[i + 1];
            }
            mName[i] = 'a';
        }
        else
        {
            break;
        }
    }
}

std::string SymbolNameCounter::current() const
{
    return { mName.rbegin(), mName.rend() };
}
}
