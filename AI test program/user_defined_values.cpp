#include <iostream>
using namespace std;

#include "user_defined_values.h"

USER_DEFINED_VALUES::USER_DEFINED_VALUES() :
	t_min(5.0),
	t_max(20.0),
	t_wrn(50.0),
	t_med(30.0),
	v_min(2.0) {}

void USER_DEFINED_VALUES::write() 
{
	cout << "User defined values: " << endl;
    cout << " - minimum green light time: " << t_min << "s" << endl;
    cout << " - maximum green light time: " << t_max << "s" << endl;
    cout << " - warning time: " << t_wrn << "s" << endl;
	cout << " - medium time: " << t_med << "s" << endl;
    cout << " - mininum detected traffic intensity: " << v_min << "cars/min" << endl;
}
