#include <fstream>

#include "Lexical/Tokenizer.hpp"

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

	std::ifstream in(argv[1]);
	Tokenizer t(in);
	t.tokenize();
	t.dumpTokens();

	getchar();
}
