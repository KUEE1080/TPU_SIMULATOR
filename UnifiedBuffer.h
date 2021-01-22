#pragma once

#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>

#include "MatrixMultiplyUnit.h"

#define MATRIX_MATRIX_MULT 0
#define VECTOR_MATRIX_MULT 1
#define ELEMENT_WISE_MATRIX_MULT 2
#define ELEMENT_WISE_VECTOR_MULT 3

extern std::vector<__int8> node_input;

void SystolicDataSetup(int mode, int input_length);
