// Assuming the unified buffer is ideal. So the code structure does not consider the hardware constraints
#include "UnifiedBuffer.h"

std::vector<__int8> node_input;

void SystolicDataSetup(int mode, int input_length) { // B x 256
	srand(time(0));
	//needs to be in 2d array
	for (int i = 0; i < input_length * MATRIX_SIZE; i++) {
		node_input.push_back((rand() % MATRIX_SIZE) - (MATRIX_SIZE / 2));
	}
}