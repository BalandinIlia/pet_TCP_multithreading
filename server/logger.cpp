#include "iostream"
#include "mutex"
#include "logger.h"

// Mutex which makes the function thread-safe
static std::mutex mutCons;

void log(const std::string& s)
{
	mutCons.lock();
	std::cout << std::endl << s;
	mutCons.unlock();
}