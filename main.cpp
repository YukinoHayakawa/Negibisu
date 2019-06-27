#include <fstream>
#include <filesystem>
#include <iostream>

#include "Lexical/Tokenizer.hpp"
#include "AST/ASTNode.hpp"
#include "AST/ScriptNode.hpp"

#ifdef _WIN32
#include <Usagi/Extension/Win32/Win32Helper.hpp>
#endif

using namespace usagi;
using namespace usagi::negibisu;

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

		fmt::print("\n");
		fmt::print("AST\n");
		fmt::print("===\n\n");

		auto token_begin = t.tokens().begin();
		auto token_end = t.tokens().end();
		ScriptNode p { token_begin, token_end };
		p.parse();
		std::string indent;
		p.print(indent);

        for(auto &s : p.sections())
        {
            fmt::print("\n");
            fmt::print("Symbol Tables: {}\n", s.scriptName());
            fmt::print("========================\n\n");

            s.symbolTable().dumpSymbols();

            fmt::print("\n");
            fmt::print("String Literals: {}\n", s.scriptName());
            fmt::print("========================\n\n");

            s.symbolTable().dumpStringLiterals();
        }
	}
	catch(const std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}
