#pragma once
#include <vector>
#include <string>
#include <math.h>
#include "MatrixMultiplyUnit.h"

#define ReLU 0
#define Sigmoid 1
#define tanh 2

extern std::vector<__int32> activation_node;
void Activation_run(int mode);