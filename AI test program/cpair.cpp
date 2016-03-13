#include "cpair.h"

bool IsCollision(const std::vector<CPAIR>& cpair, const INPUT* arg1, const INPUT* arg2)
{
	for (auto it = cpair.begin(); it != cpair.end(); it++)
		if ((it->input1 == arg1 && it->input2 == arg2) || 
			(it->input2 == arg1 && it->input1 == arg2))
			return true;
	
	return false;
}