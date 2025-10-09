#include "pch.h"
#include "solver.h"

static constexpr int delay = 10000;

std::vector<number> solve(number n)
{
	std::vector<number> ans;
	number cnt = 1;
	while (n != 0)
	{
		const number rem = n % 3;
		if (rem == 2)
		{
			ans.clear();
			Sleep(delay);
			return ans;
		}
		else if (rem == 1)
			ans.push_back(cnt);
		cnt *= 3;
		n /= 3;
	}
	Sleep(delay);
	return ans;
}