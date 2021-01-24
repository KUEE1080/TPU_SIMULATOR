#include "Analysis.h"

int number_of_idle_cell = 0; //needs to be set to zero if the cycle is done!!
bool cycle_done = false;
std::vector<int> utilization_per_cycle;
std::vector<__int8> assigned_weight; //values only used in analysis! (same as the weights in MMU), expected size: MATRIX_SIZE x MATRIX_SIZE
std::vector<__int8> transposed_weight(MATRIX_SIZE * MATRIX_SIZE);

//[개발사항] 만약 mac이 정상적으로 이렇게 곱해지는 활동이 감지되면 (dataflow만 하는 경우 X), utilize 된거라고 볼 수 있다.
void Analysis_incrementIdleRate(int cnt) {
	number_of_idle_cell = cnt;
}

void Analysis_sendCycleDone() {
	utilization_per_cycle.push_back((MATRIX_SIZE * MATRIX_SIZE) - number_of_idle_cell);
	number_of_idle_cell = 0;
}

//여기서 실제로 FC layer 계산, 즉 행렬 계산을 진행한 다음에 systolic array result랑 비교하는 과정을 거쳐서 true 또는 false를 출력한다.
bool compareResult(int _ipt_size) {
	std::vector<__int32> ground_truth(_ipt_size * MATRIX_SIZE);
	
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
	/*
	[개발사항] 이 함수는 시뮬레이션 결과를 분석하고 출력해주는 기능을 한다.
	1. 총 사이클 소요 시간
	2. 각 cell들 utilization rate의 평균
	3. 실제 행렬 알고리즘과 systolic array로 구현한 값이 일치하는지 비교해서 true false 출력
	*/

	//debugging code
	/*assigned_weight.clear();
	assigned_weight.push_back(11); assigned_weight.push_back(15); assigned_weight.push_back(19); assigned_weight.push_back(23);
	assigned_weight.push_back(12); assigned_weight.push_back(16); assigned_weight.push_back(20); assigned_weight.push_back(24);
	assigned_weight.push_back(13); assigned_weight.push_back(17); assigned_weight.push_back(21); assigned_weight.push_back(25);
	assigned_weight.push_back(14); assigned_weight.push_back(18); assigned_weight.push_back(22); assigned_weight.push_back(26);*/

	utilization_rate = ((double)(std::accumulate(utilization_per_cycle.begin(), utilization_per_cycle.end(), 0)) / (_cycle * MATRIX_SIZE * MATRIX_SIZE)) * 100;

	std::cout << std::endl; std::cout << std::endl;
	std::cout << "------------- Simulation Result -------------" << std::endl;
	std::cout << "0. Input size: " << _ipt_size << "x" << MATRIX_SIZE <<std::endl;
	std::cout << "1. Total Cycle time: " << _cycle << " cycles" << std::endl;
	std::cout << "2. Average Utilization rate of Cells: " << utilization_rate << "%" << std::endl;

	if (compareResult(_ipt_size)) { std::cout << "3. Result Comparision: Systolic Array result is SAME as the ground truth." << std::endl; }
	else { std::cout << "3. Result Comparision: Systolic Array result is NOT SAME as the ground truth." << std::endl; }
}
