#pragma once

#include <vector>
#include "input.h"

struct CPAIR
{
	INPUT* input1;
	INPUT* input2;
};

bool IsCollision(const std::vector<CPAIR>&, const INPUT*, const INPUT*);