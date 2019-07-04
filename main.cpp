#include <fstream>
#include <filesystem>
#include <iostream>

#include <boost/program_options.hpp>

#include <Usagi/Utility/Utf8Main.hpp>

#include "Lexical/Tokenizer.hpp"
#include "Parsing/ParsingContext.hpp"
#include "AST/ScriptNode.hpp"
#include "AST/PrintContext.hpp"

namespace po = boost::program_options;
namespace fs = std::filesystem;

using namespace usagi;
using namespace negi;

std::string gOutputFile;
bool gDebug = false;

class Compiler
{
    const fs::path mInputPath;
    const fs::path mOutputPath;
    std::ifstream mInputStream;
    std::ofstream mOutputStream;
    Tokenizer mTokenizer;
    ScriptNode mScript;
    PrintContext mPrintContext;
    std::ostream *mOutputTarget = &std::cout;

public:
    Compiler(
        fs::path input_path,
        fs::path output_path)
        : mInputPath(std::move(input_path))
        , mOutputPath(std::move(output_path))
        , mInputStream(mInputPath)
        , mTokenizer(mInputPath.u8string(), mInputStream)
    {
        if(!mOutputPath.empty())
        {
            mOutputStream.open(mOutputPath, std::ios::binary);
            mOutputTarget = &mOutputStream;
        }
        mPrintContext.output = mOutputTarget;
    }

#define OUTPUT(...) fmt::print(*mOutputTarget, __VA_ARGS__)
    void compile()
    {
        try
        {
            // lexical analysis
            mTokenizer.tokenize();
            if(gDebug)
            {
                OUTPUT("Token Stream\n");
                OUTPUT("============\n\n");
                mTokenizer.dumpTokens(*mOutputTarget);
            }

            // syntactic analysis
            ParsingContext pc { mTokenizer.tokens() };
            mScript.parse(&pc);
            if(gDebug)
            {
                OUTPUT("\n");
                OUTPUT("AST (Parsed)\n");
                OUTPUT("============\n\n");
                mScript.print(mPrintContext);
            }

            // semantic analysis
            mScript.check(nullptr);
            if(gDebug)
            {
                OUTPUT("\n");
                OUTPUT("AST (Checked)\n");
                OUTPUT("=============\n\n");
                mScript.print(mPrintContext);
            }

            // code generation
            for(auto &s : mScript.sections())
            {
                s.context().output = mOutputTarget;

                if(gDebug)
                {
                    OUTPUT("\n");
                    OUTPUT("Symbol Tables: {}\n",
                        s.scriptName());
                    OUTPUT("========================\n\n");

                    s.context().symbol_table.dumpSymbols(*mOutputTarget);

                    OUTPUT("\n");
                    OUTPUT("String Literals: {}\n",
                        s.scriptName());
                    OUTPUT("========================\n\n");

                    s.context().symbol_table.dumpStringLiterals(*mOutputTarget);

                    OUTPUT("\n");
                    OUTPUT("Target Code: {}\n", s.scriptName());
                    OUTPUT("========================\n\n");
                }
                s.generate(nullptr);
            }
        }
        catch(const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
};
#undef OUTPUT

int usagi_main(const std::vector<std::string> &args)
{
    po::options_description desc("Negibisu script compiler options");
    desc.add_options()
        ("help,h", "show available options")
        ("debug,d", "output parsed tokens and AST")
        ("input-file,i", po::value<std::string>(), "input file")
        ("output-file,o", po::value<std::string>(&gOutputFile), "output file")
    ;

    po::positional_options_description p;
    p.add("input-file", -1);

    po::variables_map vm;
    store(po::basic_command_line_parser<char>(args)
        .options(desc).positional(p).run(), vm, false);
    notify(vm);

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

    gDebug = vm.count("debug");

    Compiler c {
        fs::u8path(vm["input-file"].as<std::string>()),
        vm.count("output-file")
            ? fs::u8path(vm["output-file"].as<std::string>())
            : ""
    };
    c.compile();

    return 0;
}
