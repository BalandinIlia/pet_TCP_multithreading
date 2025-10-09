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
	m_mutMap.lock();
	if (m_solutions.find(num) != m_solutions.end())
	{
		auto ret = m_solutions[num];
		m_mutMap.unlock();
		return ret;
	}
	m_mutMap.unlock();

	auto ans = solve(num);
	
	m_mutMap.lock();
	if (m_solutions.find(num) != m_solutions.end())
	{
		auto ret = m_solutions[num];
		m_mutMap.unlock();
		return ret;
	}
	m_solutions[num] = ans;
	m_mutMap.unlock();
	return ans;
}

CMathCoreHost::~CMathCoreHost()
{
	delete pOne;
}