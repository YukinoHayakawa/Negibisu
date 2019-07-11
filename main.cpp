﻿#include <fstream>
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

std::string gOutputFolder;
std::string gPrefix;
bool gDebug = false;

class Compiler
{
    const fs::path mInputPath;
    const fs::path mOutputFolderPath;
    std::ifstream mInputStream;
    std::ofstream mOutputStream;
    Tokenizer mTokenizer;
    ScriptNode mScript;
    PrintContext mPrintContext;
    std::ostream *mOutputTarget = nullptr;
    
public:
    Compiler(
        fs::path input_path,
        fs::path output_folder_path)
        : mInputPath(std::move(input_path))
        , mOutputFolderPath(std::move(output_folder_path))
        , mInputStream(mInputPath)
        , mTokenizer(mInputPath.u8string(), mInputStream)
    {
        fs::create_directories(mOutputFolderPath);
        mPrintContext.output = &std::cout;
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
                auto path = mOutputFolderPath / gPrefix;
                path += s.scriptName();
                path.replace_extension(".lua");
                std::ofstream output { path };
                s.context().output = &output;

                if(gDebug)
                {
                    OUTPUT("\n");
                    OUTPUT("Symbol Tables: {}\n",
                        s.scriptName());
                    OUTPUT("========================\n\n");

                    s.context().symbol_table.dumpSymbols(output);

                    OUTPUT("\n");
                    OUTPUT("String Literals: {}\n",
                        s.scriptName());
                    OUTPUT("========================\n\n");

                    s.context().symbol_table.dumpStringLiterals(output);

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
        ("input-file,i", po::value<std::string>(), "input file path")
        ("output-folder,o", po::value<std::string>()->default_value("."), 
            "output folder which will contain all compiled section scripts")
        ("output-prefix,p", po::value<std::string>(&gPrefix)->default_value("scene_"),
            "string which will be prepended to each output filename")
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
        fs::u8path(vm["output-folder"].as<std::string>())
    };
    c.compile();

    return 0;
}
