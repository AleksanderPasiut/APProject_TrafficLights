#pragma once

#include <iostream>
#include <vector>
using namespace std;

#include "user_defined_values.h"
#include "input.h"
#include "cpair.h"

void LoadOutputs(vector<CAR_OUTPUT*>& outputs);
void LoadInputs(vector<INPUT*>& inputs, const vector<CAR_OUTPUT*>& outputs);
void SetCollisions(vector<CPAIR>& cpairs, const vector<INPUT*>& inputs);

void Algorithm(const vector<INPUT*>& inputs, const vector<CPAIR>& cpairs);