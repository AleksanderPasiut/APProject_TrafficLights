#pragma once

#include <vector>
#include "input.h"

class INPUTS_SET
{
protected:
	std::vector<INPUT*> set;

public:
	void AssignInput(INPUT* ptr) { set.push_back(ptr); }

	bool AreAllOutputsUnblocked() const;
	bool AreAllInputsAwaiting() const;
	double RetSumaryIntensity() const;
	double RetMaxAwaitingTime() const;
};