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

	input_size = 1; // (일단 디버깅용으로 이렇게 해뒀다.)just for testing

	//[가정] 여기서는 MatrixMultiply 요청이 딱 한번 있었다고 가정한다. -- 일단 개발의 편의상 그렇게 하고 추후 개선을 해나가보자.

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