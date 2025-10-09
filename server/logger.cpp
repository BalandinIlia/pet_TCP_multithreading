#include "iostream"
#include "mutex"
#include "logger.h"

static std::mutex mutCons;

void log(const std::string& s)
{
	mutCons.lock();
	std::cout << std::endl << s;
	mutCons.unlock();
}