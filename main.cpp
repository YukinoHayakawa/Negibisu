#include <fstream>
#include <filesystem>

#include "Lexical/Tokenizer.hpp"

#ifdef _WIN32
#include <Usagi/Extension/Win32/Win32Helper.hpp>
#endif

using namespace usagi;
using namespace usagi::negibisu;

int main(int argc, char *argv[])
{
	// auto runtime = Runtime::create();
	// runtime->enableCrashHandler("GAMECrashDump");
	// try
	// {
	// }
	// catch(const std::exception &e)
	// {
	// 	runtime->displayErrorDialog(e.what());
	// 	throw;
	// }

#ifdef _WIN32
	win32::patchConsole();
#endif

	const auto input_name = std::filesystem::canonical(argv[1]);
	std::ifstream in(input_name);
	Tokenizer t(input_name.u8string(), in);
	t.tokenize();
	t.dumpTokens();

	return 0;
}
