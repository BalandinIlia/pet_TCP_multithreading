#include "pch.h"
#include "host.h"
#include "solver.h"

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

std::vector<number> CMathCoreHost::get(number num)
{
	{
		// check if the solution is already known
		LG lk(m_mutMap);
		if (m_solutions.find(num) != m_solutions.end())
		{
			auto ret = m_solutions[num];
			return ret;
		}
	}

	auto ans = solve(num);
	
	LG lk(m_mutMap);
	if (m_solutions.find(num) != m_solutions.end())
	{
		auto ret = m_solutions[num];
		return ret;
	}
	m_solutions[num] = ans;
	return ans;
}

CMathCoreHost::~CMathCoreHost()
{
	delete pOne;
}