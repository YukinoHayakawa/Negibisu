#include <fstream>
#include <filesystem>
#include <iostream>

#include "Lexical/Tokenizer.hpp"
#include "AST/ScriptNode.hpp"
#include "Parsing/ParsingContext.hpp"
#include "AST/PrintContext.hpp"

#ifdef _WIN32
#include <Usagi/Extension/Win32/Win32Helper.hpp>
#endif

using namespace usagi;
using namespace usagi::negi;

int main(int argc, char *argv[])
{
#ifdef _WIN32
    win32::patchConsole();
#endif

    try
    {
        fmt::print("Token Stream\n");
        fmt::print("============\n\n");
        const auto input_name = std::filesystem::canonical(argv[1]);
        std::ifstream in(input_name);
        Tokenizer t(input_name.u8string(), in);
        t.tokenize();
        t.dumpTokens();

        ParsingContext ctx { t.tokens() };
        ScriptNode p;
        PrintContext pp { std::cout };

        fmt::print("\n");
        fmt::print("AST (Parsed)\n");
        fmt::print("============\n\n");

        p.parse(&ctx);
        p.print(pp);

        fmt::print("\n");
        fmt::print("AST (Checked)\n");
        fmt::print("============\n\n");

        // todo continue to next line after semantic error
        try
        {
            p.check(nullptr);
        }
        catch(SemanticError &)
        {
        }
        p.print(pp);

        for(auto &s : p.sections())
        {
            fmt::print("\n");
            fmt::print("Symbol Tables: {}\n", s.scriptName());
            fmt::print("========================\n\n");

            s.context().symbol_table.dumpSymbols();

            fmt::print("\n");
            fmt::print("String Literals: {}\n", s.scriptName());
            fmt::print("========================\n\n");

            s.context().symbol_table.dumpStringLiterals();
        }
    }
    catch(const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
