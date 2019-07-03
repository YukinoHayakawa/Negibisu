﻿#include <fstream>
#include <filesystem>
#include <iostream>

#include <boost/program_options.hpp>

#include "Lexical/Tokenizer.hpp"
#include "AST/ScriptNode.hpp"
#include "Parsing/ParsingContext.hpp"
#include "AST/PrintContext.hpp"

#ifdef _WIN32
#include <Usagi/Extension/Win32/Win32Helper.hpp>
#endif

namespace po = boost::program_options;
using namespace usagi;
using namespace negi;

void compileFile(const std::filesystem::path &path)
{
    try
    {
        std::ifstream in(path);
        Tokenizer t(path.u8string(), in);
        t.tokenize();
        ParsingContext ctx { t.tokens() };
        ScriptNode p;
        p.parse(&ctx);
        p.check(nullptr);

        for(auto &s : p.sections())
        {
            s.generate(nullptr);
        }
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void debugCompileFile(const std::filesystem::path &path)
{
    try
    {
        PrintContext pp { std::cout };
        fmt::print("Token Stream\n");
        fmt::print("============\n\n");
        std::ifstream in(path);
        Tokenizer t(path.u8string(), in);
        t.tokenize();
        t.dumpTokens();

        ParsingContext ctx { t.tokens() };
        ScriptNode p;

        fmt::print("\n");
        fmt::print("AST (Parsed)\n");
        fmt::print("============\n\n");

        p.parse(&ctx);
        p.print(pp);

        fmt::print("\n");
        fmt::print("AST (Checked)\n");
        fmt::print("=============\n\n");

        p.check(nullptr);
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


            fmt::print("\n");
            fmt::print("Target Code: {}\n", s.scriptName());
            fmt::print("========================\n\n");
            s.generate(nullptr);
        }
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

#ifdef _WIN32
int wmain(int argc, wchar_t *argv[])
{
    // win32::patchConsole();
#else
int main(int argc, char *argv[])
{
#endif

    // Declare the supported options.
    po::options_description desc("Negibisu script compiler options");
    desc.add_options()
        ("help,h", "show available options")
        ("debug,d", "output parsed tokens and AST")
        ("input-file,i", "input file")
    ;

    po::positional_options_description p;
    p.add("input-file", -1);

    po::variables_map vm;
    po::store(po::basic_command_line_parser<
        std::remove_pointer_t<std::remove_pointer_t<decltype(argv)>>
    >(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);

    if(vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 1;
    }
    if(!vm.count("input-file"))
    {
        std::cerr << "No input file." << std::endl;
        return 1;
    }

    const auto input_path = std::filesystem::canonical(
        vm["input-file"].as<std::string>()
    );
    if(vm.count("debug"))
        debugCompileFile(input_path);
    else
        compileFile(input_path);

    return 0;
}
