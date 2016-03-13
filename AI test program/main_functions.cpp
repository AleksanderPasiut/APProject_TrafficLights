#include <iomanip>

#include "main_functions.h"

void LoadOutputs(vector<CAR_OUTPUT*>& outputs)
{
	for (int i = 0; i < 4; i++)
		outputs.push_back(new CAR_OUTPUT(false));
}
void LoadInputs(vector<INPUT*>& inputs, const vector<CAR_OUTPUT*>& outputs)
{
#define apt(param) ptr->AssignOutput(outputs[param])
#define pb inputs.push_back(ptr)

	CAR_INPUT* ptr;

	ptr = new CAR_INPUT(0, true, 20.0); pb; apt(2); apt(3); 
	ptr = new CAR_INPUT(1, true, 40.0); pb; apt(2); 
	ptr = new CAR_INPUT(2, true, 30.0); pb; apt(1);
	ptr = new CAR_INPUT(3, true, 10.0); pb; apt(0); apt(3);
	ptr = new CAR_INPUT(4, true, 10.0); pb; apt(3);
	ptr = new CAR_INPUT(5, true, 20.0); pb; apt(0); apt(1);
	ptr = new CAR_INPUT(6, true, 40.0); pb; apt(0);
	ptr = new CAR_INPUT(7, true, 30.0); pb; apt(3);
	ptr = new CAR_INPUT(8, true, 10.0); pb; apt(1); apt(2);
	ptr = new CAR_INPUT(9, true, 10.0); pb; apt(1);

	for (int i = 0; i < 4; i++)
		inputs.push_back(new PEDESTRIAN_CROSSING(10+i, true)); 
}
void SetCollisions(vector<CPAIR>& cpairs, const vector<INPUT*>& inputs)
{
#define sc(one,two) cpairs.push_back({inputs[one], inputs[two]})
	sc(0,3);
	sc(0,4);
	sc(0,7);
	sc(0,8);
	sc(0,9);
	sc(0,10);
	sc(0,12);
	sc(1,3);
	sc(1,4);
	sc(1,7);
	sc(1,8);
	sc(1,9);
	sc(1,10);
	sc(1,12);
	sc(2,3);
	sc(2,4);
	sc(2,8);
	sc(2,9);
	sc(2,10);
	sc(2,11);
	sc(3,5);
	sc(3,6);
	sc(3,7);
	sc(3,9);
	sc(3,11);
	sc(3,13);
	sc(4,5);
	sc(4,6);
	sc(4,7);
	sc(4,8);
	sc(4,11);
	sc(4,12);
	sc(5,8);
	sc(5,9);
	sc(5,10);
	sc(5,12);
	sc(6,8);
	sc(6,9);
	sc(6,10);
	sc(6,12);
	sc(7,8);
	sc(7,9);
	sc(7,12);
	sc(7,13);
	sc(8,11);
	sc(8,13);
	sc(9,10);
	sc(9,13);
}

void NoExceededTime(vector<INPUT*>& out, const vector<INPUT*>& inputs)
{
	for (auto it = inputs.begin(); it != inputs.end(); it++)
		if ((*it)->RetAwaiting() &&
			(*it)->RetAwaitingTime() <= udv.RetTWrn())
			out.push_back(*it);
	return;
}
void ExceededTime(vector<INPUT*>& out, const vector<INPUT*>& inputs)
{
	for (auto it = inputs.begin(); it != inputs.end(); it++)
		if ((*it)->RetAwaiting() &&
			(*it)->RetAwaitingTime() > udv.RetTWrn())
			out.push_back(*it);
	return;
}
void Sort(vector<INPUT*>& out)
{
	for (auto it = out.begin(); it != out.end(); it++)
		for (auto jt = out.begin(); jt+1 != out.end(); jt++)
			if ((*jt)->RetAwaitingTime() < (*(jt+1))->RetAwaitingTime())
			{
				INPUT* tmp = *jt;
				*(jt._Ptr) = *(jt+1);
				*((jt+1)._Ptr) = *jt;
			}
}
void Reduce(vector<INPUT*>& out, const vector<CPAIR>& cpairs)
{
	for (auto it = out.begin(); it != out.end(); )
	{
		bool collision = false;
		for (auto jt = out.begin(); jt != it; jt++)
			if (IsCollision(cpairs, *it, *jt))
			{
				it = out.erase(it);
				collision = true;
				break;
			}

		if (!collision)
			it++;
	}
}
void ReduceNET(vector<INPUT*>& out, const vector<INPUT*>& exceeded, const vector<CPAIR>& cpairs)
{
	for (auto it = out.begin(); it != out.end();)
	{
		bool collision = false;
		for (auto jt = exceeded.begin(); jt != exceeded.end(); jt++)
			if (IsCollision(cpairs, *it, *jt))
			{
				it = out.erase(it);
				collision = true;
				break;
			}

		if (!collision)
			it++;
	}
}

void PickSubset(vector<INPUT*>& subset, const vector<INPUT*>& set, unsigned __int64 set_number)
{
	for (unsigned __int64 marker = 1, i = 0; i < set.size(); marker <<= 1, i++)
		if (marker & set_number)
			subset.push_back(set[i]);
}
bool IsSetCollisive(const vector<INPUT*>& set, const vector<CPAIR>& cpairs)
{
	for (auto it = set.begin(); it != set.end(); it++)
		for (auto jt = set.begin(); jt != it; jt++)
			if (IsCollision(cpairs, *it, *jt))
				return true;

	return false;
}
double ComputePriority(const vector<INPUT*>& set)
{
	double ret = 0.0;
	for (auto it = set.begin(); it != set.end(); it++)
		if ((*it)->RetAwaiting())
			ret += (*it)->RetAwaitingTime()*(*it)->RetIntensity();

	return ret;
}
void Choose(vector<INPUT*>& out, const vector<CPAIR>& cpairs)
{
	vector<INPUT*> new_set;
	double priority = 0.0;

	unsigned __int64 limit = 1 << out.size();
	for (unsigned __int64 c = 1; c < limit; c++)
	{
		vector<INPUT*> subset;
		PickSubset(subset, out, c);

		if (!IsSetCollisive(subset, cpairs))
		{
			double subset_priority = ComputePriority(subset);
			if (priority < subset_priority)
			{
				priority = subset_priority;
				new_set = subset;
			}
		}
	}

	out = new_set;
}
void Combine(vector<INPUT*>& set1, const vector<INPUT*>& set2)
{
	set1.insert(set1.end(), set2.begin(), set2.end());
}

double ComputeGreenLightTime(const vector<INPUT*>& new_set, const vector<INPUT*>& inputs)
{
	double set_sum = 0.0;
	for (auto it = new_set.begin(); it != new_set.end(); it++)
		set_sum += (*it)->RetIntensity();

	double all_sum = 0.0;
	for (auto it = inputs.begin(); it != inputs.end(); it++)
		all_sum += (*it)->RetIntensity();

	return min(max(udv.RetTMed()*set_sum/all_sum, udv.RetTMin()), udv.RetTMax());
}
void WriteLog(double t, const vector<INPUT*>& new_set, const vector<INPUT*>& inputs)
{
	cout << setw(6) << t;

	for (auto it = inputs.begin(); it != inputs.end(); it++)
	{
		bool iins = false;

		for (auto jt = new_set.begin(); jt != new_set.end(); jt++)
			if (*it == *jt)
			{
				iins = true;
				break;
			}

		cout.precision(3);
		if (iins)
			cout << setw(5) << (*it)->RetAwaitingTime();
		else cout << " --- ";
	}
	cout << endl;
}
void Proceed(const vector<INPUT*>& new_set, const vector<INPUT*>& inputs)
{
	double t = ComputeGreenLightTime(new_set, inputs);

	WriteLog(t, new_set, inputs);

	for (auto it = inputs.begin(); it != inputs.end(); it++)
		(*it)->SetAwaitingTime((*it)->RetAwaitingTime()+t);

	for (auto it = new_set.begin(); it != new_set.end(); it++)
		(*it)->SetAwaitingTime(0.0);
}

void Algorithm(const vector<INPUT*>& inputs, const vector<CPAIR>& cpairs)
{
	// wybieram wloty o nieprzekroczonym czasie oczekiwania
	vector<INPUT*> no_exceeded_time;
	NoExceededTime(no_exceeded_time, inputs);

	// wybieram wloty o przekroczonym czasie oczekiwania
	vector<INPUT*> exceeded_time;
	ExceededTime(exceeded_time, inputs);

	// je¿eli gdzieœ czas oczekiwania zosta³ przekroczony
	if (exceeded_time.size())
	{
		// sortowanie wed³ug czasu oczekiwania malej¹co
		Sort(exceeded_time);
		
		// redukcja: pozostaje pierwszy element i najlepszy zestaw niekoliduj¹cych z nim
		Reduce(exceeded_time, cpairs);

		// redukcja: pozostaj¹ elementy, które nie koliduj¹ ze zbiorem exceeded_time
		ReduceNET(no_exceeded_time, exceeded_time, cpairs);

		// wybór najlepszego zestawu spoœród no_exceeded_time
		Choose(no_exceeded_time, cpairs);

		// po³¹czenie zestawów exceeded_time i no_exceeded_time
		Combine(no_exceeded_time, exceeded_time);
	}
	// je¿eli czas oczekiwania nigdzie nie jest przekroczony
	else
	{
		// wybór najlepszego zestawu spoœród no_exceeded_time
		Choose(no_exceeded_time, cpairs);
	}

	// przeprowadzanie zmiany œwiate³
	Proceed(no_exceeded_time, inputs);
}