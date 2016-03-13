#pragma once

#include <algorithm>
#include <vector>

#include "user_defined_values.h"
#include "output.h"

class INPUT
{
protected:
	unsigned id;
	bool awaiting; // are there any pedestrians or cars awaiting?
	double awaiting_time; // time that pedestrians or cars await on this input (in seconds)

public:
	INPUT(unsigned id, bool default_awaiting);
	void SetAwaiting(bool arg) { awaiting = arg; }
	void SetAwaitingTime(double arg) { awaiting_time = arg; }

	bool RetAwaiting() const { return awaiting; }
	double RetAwaitingTime() const { return awaiting_time; }
	virtual double RetIntensity() const = 0;
	virtual bool AreOutputsBlocked() const = 0;
};

class PEDESTRIAN_CROSSING : public INPUT
{
public:
	PEDESTRIAN_CROSSING(unsigned id, bool default_awaiting);
	double RetIntensity() const { return awaiting ? udv.RetVMin() : 0.0; }
	bool AreOutputsBlocked() const { return false; }
};

class CAR_INPUT : public INPUT
{
protected:
	std::vector<CAR_OUTPUT*> outputs; // outputs for this input

	double intensity; // traffic intensity

public:
	CAR_INPUT(unsigned id, bool default_awaiting, double default_intensity);
	void AssignOutput(CAR_OUTPUT* output_ptr) { outputs.push_back(output_ptr); }
	void SetIntensity(double arg) { intensity = arg; }
	double RetIntensity() const { return awaiting ? std::max(intensity, udv.RetVMin()) : 0.0; }
	bool AreOutputsBlocked() const;
};
