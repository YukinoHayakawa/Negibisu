#include <fstream>
#include <filesystem>
#include <iostream>

#include <boost/program_options.hpp>

#include <Usagi/Utility/Utf8Main.hpp>

#include "Lexical/Tokenizer.hpp"
#include "Parsing/ParsingContext.hpp"
#include "AST/ScriptNode.hpp"
#include "AST/PrintContext.hpp"
#include "AST/Statement/IntrinsicCommand.hpp"

namespace po = boost::program_options;
namespace fs = std::filesystem;

using namespace usagi;
using namespace negi;

std::string gOutputFolder;
std::string gNarratorName;
// std::string gPrefix;
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
    std::ostream *mDebugOutput = &std::cout;

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
        mPrintContext.output = mDebugOutput;
        mInputStream.exceptions(std::ios::badbit);
    }

#define OUTPUT(...) fmt::print(*mDebugOutput, __VA_ARGS__)
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
                mTokenizer.dumpTokens(*mDebugOutput);
            }

            // syntactic analysis
            ParsingContext pc { mTokenizer.tokens() };
            // setup config
            pc.config_narrator_name = gNarratorName;

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
                auto path = mOutputFolderPath; // / gPrefix;
                path /= s.scriptName();
                path.replace_extension(".lua");
                std::ofstream output { path, std::ios::binary };
                path.replace_extension(".asset-refs.txt");
                std::ofstream asset_ref { path, std::ios::binary };
                s.context().output = &output;

                if(gDebug)
                {
                    OUTPUT("\n");
                    OUTPUT("Symbol Tables: {}\n",
                        s.scriptName());
                    OUTPUT("========================\n\n");

                    s.context().symbol_table.dumpSymbols(*mDebugOutput);

                    OUTPUT("\n");
                    OUTPUT("String Literals: {}\n",
                        s.scriptName());
                    OUTPUT("========================\n\n");

                    s.context().symbol_table.dumpStringLiterals(*mDebugOutput);

                    // OUTPUT("\n");
                    // OUTPUT("Referenced Assets: {}\n",
                    //     s.scriptName());
                    // OUTPUT("========================\n\n");
                    //
                    // s.context().symbol_table.
                    // OUTPUT("\n");
                    // OUTPUT("Target Code: {}\n", s.scriptName());
                    // OUTPUT("========================\n\n");
                }
                s.generate(nullptr);
                s.context().symbol_table.dumpAssetRefs(asset_ref);
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
        // help
        ("help,h",
            "show available options")
        ("list-commands,c",
            "list available commands and exit")
        // debugging
        ("debug,d",
            "output parsed tokens and AST")
        ("input-file,i",
            po::value<std::string>(),
            "input file path")
        ("output-folder,o",
            po::value<std::string>()->default_value("."),
            "output folder which will contain all compiled section scripts")
        // scene config
        ("narrator-name,nn",
            po::value<std::string>(&gNarratorName)->default_value("Narrator"),
            "set narrator name")
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
        return 0;
    }
    if(vm.count("list-commands"))
    {
        outputAvailableCommands();
        return 0;
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
