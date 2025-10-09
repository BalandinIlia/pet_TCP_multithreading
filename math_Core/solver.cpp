#include "pch.h"
#include "../controls/controls.h"
#include "solver.h"

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
			Sleep(delayAlgo);
			return ans;
		}
		else if (rem == 1)
			ans.push_back(cnt);
		cnt *= 3;
		n /= 3;
	}
	Sleep(delayAlgo);
	return ans;
}