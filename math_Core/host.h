#pragma once
#include "pch.h"
#include "../shared/params.h"

class CMathCoreHost
{
private:
	CMathCoreHost();
	static CMathCoreHost* pOne;
public:
	static CMathCoreHost& one();

	std::vector<number> get(number num);

	~CMathCoreHost();

private:
	std::map<number, std::vector<number>> m_solutions;

	std::mutex m_mutMap;
};