#include <iostream>
#include <time.h>
#include <fstream>

#include "vsfp.h"
using namespace std;

double t_ps(double n)
{
	return 2.15*n;
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
void PrepareInversedMatrices(MATRIX<double> invB[4], const MATRIX<double> B[4])
{
	const unsigned n = 10;
	const unsigned m = 4;

	for (unsigned k = 0; k < m; k++)
	{
		invB[k].change_size(m, m);

		for (unsigned p = 0; p < m; p++)
			for (unsigned q = 0; q < m; q++)
				for (unsigned i = 0; i < n; i++)
					invB[k].field(p, q) += B[k].field(i, p)*B[k].field(i, q);

		invB[k] = MATRIX_SoLE<double>::matrix_inverse(invB[k]);
	}
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

void PrepareFactorVector(MATRIX<double>& vector, const MATRIX<double>& v, const MATRIX<double>& B)
{
	for (unsigned k = 0; k < 4; k++)
	{
		vector.field(k) = 0;

		for (unsigned i = 0; i < 10; i++)
			vector.field(k) += B.field(i, k)*v.field(i);
	}
}
void ComputeCycleTime(double& t_cycl, const MATRIX<double>& v, const MATRIX<double>& B, double t_z)
{
	double u = 0;
	for (unsigned j = 0; j < 4; j++)
	{
		double tmp = 0;
		for (unsigned i = 0; i < 10; i++)
			if (tmp < B.field(i,j)*t_ps(v.field(i)))
				tmp = B.field(i,j)*t_ps(v.field(i));

		u += tmp;
	}

	if (t_cycl >= u+4*t_z)
		t_cycl  = u+4*t_z;
}
void PerformScaling(MATRIX<double>& x, double& s, double t_cycl, double t_z)
{
	s = t_cycl-4*t_z;

	double sum = 0;
	for (unsigned i = 0; i < 4; i++)
		sum += x.field(i);

	s/= sum;
	x*= s;
}
void PerformLimiting(MATRIX<double>& x, double t_min, double t_max, const MATRIX<double>& vector)
{
	for (unsigned i = 0; i < 4; i++)
	{
		if (vector.field(i) == 0)
			x.field(i) = 0;
		else if (x.field(i) < t_min)
			x.field(i) = t_min;
		else if (x.field(i) > t_max)
			x.field(i) = t_max;
	}
}
double ComputeError(const MATRIX<double>& B, const MATRIX<double>& x, const MATRIX<double>& v, double s)
{
	MATRIX<double> dv = B*x-v*s;
	double deltaV = 0;
	for (unsigned i = 0; i < dv.rows(); i++)
		deltaV += dv.field(i)*dv.field(i);
	
	return sqrt(deltaV);
}

void algorithm(
	const MATRIX<double>& v,
	const MATRIX<double> B[4],
	const MATRIX<double> invB[4],
	double t_min,
	double t_max,
	double& t_cycl,
	double t_z,
	MATRIX<double>& x,
	double& deltaV,
	unsigned& id,
	double& s)
{
	deltaV = HUGE_VAL;

	for (unsigned ver = 0; ver < 4; ver++)
	{
		MATRIX<double> vector(4);
		PrepareFactorVector(vector, v, B[ver]);

		MATRIX<double> u = invB[ver]*vector;

		ComputeCycleTime(t_cycl, v, B[ver], t_z);

		PerformScaling(u, s, t_cycl, t_z);
		PerformLimiting(u, t_min, t_max, vector);

		double ndV = ComputeError(B[ver], u, v, s);
		if (ver == 0 || ndV < deltaV)
		{
			deltaV = ndV;
			x = u;
			id = ver;
		}
	}
}

void WriteDataToFile(fstream& F, const MATRIX<double> v, const MATRIX<double> B, const MATRIX<double> x, double deltaV, double s, double t_cycl)
{
	for (unsigned i = 0; i < v.rows(); i++)
		F << v.field(i) << "	";
	
	MATRIX<double> real = B*x;
	for (unsigned i = 0; i < real.rows(); i++)
		F << real.field(i) << "	";

	F << deltaV << "	" << s << "	" << t_cycl << endl;
}
void WriteOnScreen(const MATRIX<double> v, const MATRIX<double> B, const MATRIX<double> x, double deltaV, double s, unsigned id, double t_cycl)
{
	MATRIX<double> real = B*x;
	for (unsigned i = 0; i < real.rows(); i++)
		cout << "in" << i << " " << v.field(i) << " " << real.field(i) << endl;

	cout << "deltaV: " << deltaV << endl;
	cout << "s: " << s << endl;
	cout << "id: " << id << endl;
	cout << "t_cycl: " << t_cycl << endl;
}

int main()
{
	srand(time(NULL));

	MATRIX<double> B[4];
	PrepareMainMatrices(B);

	MATRIX<double> invB[4];
	PrepareInversedMatrices(invB, B);

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

		// output vector and error value
		MATRIX<double> x(4);
		double deltaV;
		unsigned id;
		double s;
	
		algorithm(v, B, invB, t_min, t_max, t_cycl, t_z, x, deltaV, id, s);

		//WriteOnScreen(v, B[id], x, deltaV, s, id, t_cycl);

		WriteDataToFile(F, v, B[id], x, deltaV, s, t_cycl);
	}

	cout << "computing finished" << endl;
	cin.get();
	cin.ignore();
	return 0;
}