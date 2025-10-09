#include "pch.h"
#include "host.h"

CMathCoreHost* CMathCoreHost::pOne = nullptr;

CMathCoreHost::CMathCoreHost() {}

CMathCoreHost& CMathCoreHost::one()
{
	if (pOne == nullptr)
	{
		pOne = new CMathCoreHost;
	}
	return *pOne;
}

std::vector<number> CMathCoreHost::solve(number num) const
{
	std::vector<number> v;
	return v;
}

CMathCoreHost::~CMathCoreHost()
{
	delete pOne;
}