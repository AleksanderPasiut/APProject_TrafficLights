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
	cout << "traffic lights steering pattern" << endl;

	for(int i = 0; i < 30; i++)
		Algorithm(inputs, cpairs);



    return 0;
}
