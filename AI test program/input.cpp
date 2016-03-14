#include "input.h"

INPUT::INPUT(unsigned in_id, bool default_awaiting) :
	id(in_id),
	awaiting(default_awaiting),
	awaiting_time(udv.RetTYlw()),
	total_green_light_time(0.0)
{}

PEDESTRIAN_CROSSING::PEDESTRIAN_CROSSING(unsigned in_id, bool default_awaiting) :
	INPUT(in_id, default_awaiting)
{}

CAR_INPUT::CAR_INPUT(unsigned in_id, bool default_awaiting, double default_intensity) :
	INPUT(in_id, default_awaiting),
	intensity(default_intensity)
{}

bool CAR_INPUT::AreOutputsBlocked() const
{
	if (outputs.size())
		for (auto it = outputs.begin(); it != outputs.end(); it++)
			if((*it)->IsOutputBlocked())
				return true;

	return false;
}