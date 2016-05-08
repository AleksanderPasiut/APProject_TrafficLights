#include <iostream>
#include <time.h>
#include <fstream>

#include "vsfp.h"
using namespace std;

double t_ps(double n)
{
	return 2.15*n+3.65;
}
double n(double tps)
{
	return tps != 0 ? (tps-3.65) / 2.15 : 0;
}

void PrepareMainMatrices(MATRIX<double> B[4])
{
	for (unsigned i = 0; i < 4; i++)
		B[i].change_size(10, 4);

	B[0].field(0, 0) = B[0].field(1, 0) = B[0].field(5, 0) = B[0].field(6, 0) = 
		B[0].field(2, 1) = B[0].field(7, 1) = 
		B[0].field(3, 2) = B[0].field(8, 2) = 
		B[0].field(4, 3) = B[0].field(9, 3) = 1;

	B[1].field(0, 0) = B[1].field(1, 0) = B[1].field(2, 0) =
		B[1].field(5, 1) = B[1].field(6, 1) = B[1].field(7, 1) = 
		B[1].field(3, 2) = B[1].field(8, 2) = 
		B[1].field(4, 3) = B[1].field(9, 3) = 1;

	B[2].field(0, 0) = B[2].field(1, 0) = B[2].field(5, 0) = B[2].field(6, 0) = 
		B[2].field(2, 1) = B[2].field(7, 1) = 
		B[2].field(3, 2) = B[2].field(4, 2) = 
		B[2].field(8, 3) = B[2].field(9, 3) = 1;

	B[3].field(0, 0) = B[3].field(1, 0) = B[3].field(2, 0) =
		B[3].field(5, 1) = B[3].field(6, 1) = B[3].field(7, 1) = 
		B[3].field(3, 2) = B[3].field(4, 2) = 
		B[3].field(8, 3) = B[3].field(9, 3) = 1;
}
void PrepareInputVector(MATRIX<double>& v)
{
	if (true)
	{
		for (unsigned i = 0; i < v.rows(); i++)
			v.field(i) = static_cast<double>(rand() % 11);
	}
	else
	{
		cout << "Type input vector: ";
		cin >> v;
	}
}

void compute_max_cars_waiting(const MATRIX<double>& B, const MATRIX<double>& v, MATRIX<double>& m)
{
	for (unsigned j = 0; j < B.cols(); j++)
	{
		m.field(j) = 0;
		for (unsigned i = 0; i < B.rows(); i++)
			if (m.field(j) < B.field(i, j)*v.field(i))
				m.field(j) = B.field(i, j)*v.field(i);
	}
}
double compute_desired_cycle_time(const MATRIX<double>& m)
{
	double val = 0;
	for (unsigned i = 0; i < m.rows(); i++)
		val += t_ps(m.field(i));

	return val;
}
void compute_phase_times(const MATRIX<double>& m, double u, double t_cycl, MATRIX<double>& x)
{
	double sum_m = 0;
	for (unsigned i = 0; i < m.rows(); i++)
		sum_m += m.field(i);

	for (unsigned i = 0; i < m.rows(); i++)
		x.field(i) = u*t_cycl*m.field(i) / sum_m;
}
void limit_phase_times(const MATRIX<double>& m, MATRIX<double>& x, double t_min, double t_max)
{
	for (unsigned i = 0; i < 4; i++)
	{
		if (m.field(i) == 0)
			x.field(i) = 0;
		else if (x.field(i) < t_min)
			x.field(i) = t_min;
		else if (x.field(i) > t_max)
			x.field(i) = t_max;
	}
}
double compute_eff(const MATRIX<double>& B, const MATRIX<double>& v, const MATRIX<double>& x)
{
	double eff = 0;

	for (unsigned j = 0; j < B.cols(); j++)
		for (unsigned i = 0; i < B.rows(); i++)
			eff += min(n(B.field(i, j)*x.field(j)), v.field(i));

	return eff;
}

double algorithm(
	const MATRIX<double>& v,
	const MATRIX<double> B[4],
	double t_min,
	double t_max,
	double& t_cm,
	double t_z,
	MATRIX<double>& x,
	unsigned& id)
{
	MATRIX<double> tmp_x(4);
	double eff = 0;

	for (unsigned i = 0; i < 4; i++)
	{


		MATRIX<double> m(4);
		compute_max_cars_waiting(B[i], v, m);

		double t_cycl = compute_desired_cycle_time(m);
		double u = (t_cm - 4 * t_z) < t_cycl ? (t_cm - 4 * t_z) / t_cycl : 1;

		compute_phase_times(m, u, t_cycl, tmp_x);
		limit_phase_times(m, tmp_x, t_min, t_max);

		double tmp_eff = compute_eff(B[i], v, tmp_x);




		if (i == 0 || eff < tmp_eff)
		{
			x = tmp_x;
			eff = tmp_eff;
			id = i;
		}
	}

	return eff;
}

void WriteDataToFile(fstream& F, const MATRIX<double> v, const MATRIX<double> B, const MATRIX<double> x, double t_cycl)
{
	for (unsigned i = 0; i < v.rows(); i++)
		F << v.field(i) << "	";
	
	MATRIX<double> real = B*x;
	for (unsigned i = 0; i < real.rows(); i++)
		F << real.field(i) << "	";

	F << t_cycl << endl;
}
void WriteOnScreen(const MATRIX<double> v, const MATRIX<double> B, const MATRIX<double> x, unsigned id, double t_cycl, double eff)
{
	MATRIX<double> real = B*x;
	for (unsigned i = 0; i < real.rows(); i++)
		cout << "in" << i << " " << v.field(i) << " " << real.field(i) << endl;

	cout << "id: " << id << endl;
	cout << "t_cycl: " << t_cycl << endl;
	cout << "eff: " << eff << endl;

	cin.get();
	cin.ignore();
}

int main()
{
	srand(time(NULL));

	MATRIX<double> B[4];
	PrepareMainMatrices(B);

	fstream F("out.txt", fstream::out);

	cout << "computing started" << endl;

	for (unsigned i = 0; i < 25000; i++)
	{
		double t_min = 3.0f;
		double t_max = 40.0f;
		double t_cycl = 60.0f;
		double t_z = 4.0f;

		MATRIX<double> v(10);
		PrepareInputVector(v);

		// cycle id and output vector
		unsigned id;
		MATRIX<double> x(4);
	
		double eff = algorithm(v, B, t_min, t_max, t_cycl, t_z, x, id);

		WriteOnScreen(v, B[id], x, id, t_cycl, eff);

		//WriteDataToFile(F, v, B[id], x, deltaV, s, t_cycl);
	}

	cout << "computing finished" << endl;
	cin.get();
	cin.ignore();
	return 0;
}