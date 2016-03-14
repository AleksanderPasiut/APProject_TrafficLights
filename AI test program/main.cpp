#include <iostream>
#include <vector>
using namespace std;

#include "user_defined_values.h"
#include "input.h"
#include "cpair.h"

#include "main_functions.h"

int main()
{
    cout << "Traffic lights AI test program" << endl;
    cout << endl;
    
	udv.write();

	vector<CAR_OUTPUT*> outputs; // all car outputs
	LoadOutputs(outputs);

	vector<INPUT*> inputs; // all car inputs and pedestrian crossings
	LoadInputs(inputs, outputs);

	vector<CPAIR> cpairs; // all pair of inputs which collide with each other
	SetCollisions(cpairs, inputs);

	cout << endl;
	cout << "Traffic lights steering pattern: " << endl;

	double tst = 0.0;

	for(int i = 0; i < 30; i++)
		Algorithm(inputs, cpairs, tst);

	cout << endl;
	cout << "Total simulation time: " << tst << "s" << endl;
	TotalTrafficIntensity(inputs);
	InputsDetails(inputs);

    return 0;
}
