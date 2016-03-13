#include "inputs_set.h"

bool INPUTS_SET::AreAllOutputsUnblocked() const
{
	if (set.size())
		for (auto it = set.begin(); it != set.end(); it++)
			if ((*it)->AreOutputsBlocked())
				return false;

	return true;
}

bool INPUTS_SET::AreAllInputsAwaiting() const
{
	if (set.size())
		for (auto it = set.begin(); it != set.end(); it++)
			if (!(*it)->RetAwaiting())
				return false;

	return true;
}

double INPUTS_SET::RetSumaryIntensity() const
{
	double ret = 0.0f;

	if (set.size())
		for (auto it = set.begin(); it != set.end(); it++)
			ret += (*it)->RetIntensity();

	return ret;
}

double INPUTS_SET::RetMaxAwaitingTime() const
{
	double ret = 0.0f;

	if (set.size())
		for (auto it = set.begin(); it != set.end(); it++)
			if (ret < (*it)->RetAwaitingTime())
				ret = (*it)->RetAwaitingTime();

	return ret;
}