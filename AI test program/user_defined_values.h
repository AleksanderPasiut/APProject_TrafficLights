#pragma once

class USER_DEFINED_VALUES
{
protected:
	double t_min; // minimum green light time (in seconds)
	double t_max; // maximum green light time (in seconds)
	double t_wrn; // warning time (in seconds)
	double v_min; // minimum detected traffic intensity (in cars per minute)

public:
	USER_DEFINED_VALUES();
	void write();

	double RetTMin() const { return t_min; }
	double RetTMax() const { return t_max; }
	double RetTWrn() const { return t_wrn; }
	double RetVMin() const { return v_min; }
};

static USER_DEFINED_VALUES udv;

