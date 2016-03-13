#pragma once

class CAR_OUTPUT
{
protected:
	bool blocked;

public:
	CAR_OUTPUT(bool is_blocked) : blocked(is_blocked) {}

	void SetBlocked(bool arg) { blocked = arg; }
	bool IsOutputBlocked() { return blocked; }
};