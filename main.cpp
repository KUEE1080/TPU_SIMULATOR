// Title: TPU Simulator v0
// Author : Lee Dong Jae
// Description : TPU simulation focused on the systolic array, assuming the memory conditions are ideal. Supports only Fully Connected Layer Inference
// Input : clock speed & input matrix size (the values will be all randomized)
// Output:
// 1. shows the input, weights, and the output of the matrix
// 2. the utilization of the processing elements inside the matrix multiply unit

//############# Code segment #################

#include <iostream> 
#include "MatrixMultiplyUnit.h"
#include "UnifiedBuffer.h"
#include "Activation.h"

int input_size;

int main(void) {
	int cycle = 0;

	//initialize initial values and the settings of the TPU
	//std::cout << "The size of the Input" << std::endl; // assuming the weight is input_size * input_size
	//std::cin >> input_size;

	input_size = 1; // (�ϴ� ���������� �̷��� �ص״�.)just for testing

	//[����] ���⼭�� MatrixMultiply ��û�� �� �ѹ� �־��ٰ� �����Ѵ�. -- �ϴ� ������ ���ǻ� �׷��� �ϰ� ���� ������ �س�������.

	//code operation order: memory related initialization done first, and then operate the MMU initialization

	/*
	[Matrix Multiplication Operation Mode]
	1. MATRIX_MATRIX_MULT => Input: Matrix, Weight: Matrix
	2. VECTOR_MATRIX_MULT => Input: Vector, Weight: Matrix
	3. ELEMENT_WISE_MATRIX_MULT => Input: Matrix, Weight: Matrix
	4. ELEMENT_WISE_VECTOR_MULT => Input: Vector, Weight: Matrix
	*/

	SystolicDataSetup(VECTOR_MATRIX_MULT,input_size);

	MMU_initialize(VECTOR_MATRIX_MULT, input_size);
	//MMU_assignWeight(input_size); // assuming weight size is 256 x 256

	while (MMU_run() == IN_PROGRESS) {
		cycle++;
	}
	//int d = mmu_accumulator[0];
	Activation_run(ReLU);
	int d1 = activation_node[0];

	return 0;
}