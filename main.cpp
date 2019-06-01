#include <fstream>

#include <boost/program_options.hpp>

#include <Usagi/Runtime/Runtime.hpp>


#include "Tokenizer.hpp"

using namespace boost::program_options;
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
	Tokenizer t;
	t.tokenize(in);
	t.dumpTokens();
}
