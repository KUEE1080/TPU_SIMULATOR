#include "Analysis.h"

int number_of_idle_cell = 0; //needs to be set to zero if the cycle is done!!
bool cycle_done = false;
std::vector<int> utilization_per_cycle;
std::vector<int8_t> assigned_weight; //values only used in analysis! (same as the weights in MMU), expected size: MATRIX_SIZE x MATRIX_SIZE
std::vector<int8_t> transposed_weight(MATRIX_SIZE * MATRIX_SIZE);

void Analysis_incrementIdleRate(int cnt) {
	number_of_idle_cell = cnt;
}

void Analysis_sendCycleDone() {
	utilization_per_cycle.push_back((MATRIX_SIZE * MATRIX_SIZE) - number_of_idle_cell);
	number_of_idle_cell = 0;
}

bool compareResult(int _ipt_size) {
	std::vector<int32_t> ground_truth(_ipt_size * MATRIX_SIZE);
	
	//Step1: weight transpose
	for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {
		int a = i / MATRIX_SIZE;
		int b = i % MATRIX_SIZE;
		transposed_weight[i] = assigned_weight[MATRIX_SIZE * b + a];
	}

	//Step2: matrix multiplication (generating ground truth) .. assigned_weight is already transposed
	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < _ipt_size; j++) {
			for (int k = 0; k < MATRIX_SIZE; k++) {
				ground_truth[i * _ipt_size + j] += transposed_weight[i * MATRIX_SIZE + k] * node_input[k * _ipt_size + j];
			}
		}
	}

	int aa = ground_truth[0] + Accumulator[0][0];

	//Step3: Compare with ground truth and the accumulator
	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < _ipt_size; j++) {
			if (ground_truth[_ipt_size * i + j] != Accumulator[i][j])
				return false;
		}
	}

	return true;
}

void runAnalysis(int _ipt_size, int _cycle) {
	double utilization_rate = 0;

	utilization_rate = ((double)(std::accumulate(utilization_per_cycle.begin(), utilization_per_cycle.end(), 0)) / (_cycle * MATRIX_SIZE * MATRIX_SIZE)) * 100;

	std::cout << std::endl; std::cout << std::endl;
	std::cout << "------------- Simulation Result -------------" << std::endl;
	std::cout << "0. Input size: " << _ipt_size << "x" << MATRIX_SIZE <<std::endl;
	std::cout << "1. Total Cycle time: " << _cycle << " cycles" << std::endl;
	std::cout << "2. Average Utilization rate of Cells: " << utilization_rate << "%" << std::endl;

	if (compareResult(_ipt_size)) { std::cout << "3. Result Comparision: Systolic Array result is SAME as the ground truth." << std::endl; }
	else { std::cout << "3. Result Comparision: Systolic Array result is NOT SAME as the ground truth." << std::endl; }
}
