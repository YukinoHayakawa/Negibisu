#include <Usagi/Runtime/Runtime.hpp>


using namespace usagi;

int main(int argc, char *argv[])
{
	auto runtime = Runtime::create();
	runtime->enableCrashHandler("GAMECrashDump");
	try
	{
	}
	catch(const std::exception &e)
	{
		runtime->displayErrorDialog(e.what());
		throw;
	}
}
