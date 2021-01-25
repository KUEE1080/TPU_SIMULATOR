#pragma once

#include <iostream> 
#include <vector>
#include <algorithm>
#include <numeric>
#include <inttypes.h>

#include "MatrixMultiplyUnit.h"

extern std::vector<int> utilization_per_cycle;
extern std::vector<int8_t> assigned_weight;

//used in MMU_run
void Analysis_incrementIdleRate(int cnt);
void Analysis_sendCycleDone();

//used in main.cpp
void runAnalysis(int _ipt_size, int _cycle);