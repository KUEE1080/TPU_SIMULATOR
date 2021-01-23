// Assuming the unified buffer is ideal. So the code structure does not consider the hardware constraints
#include "UnifiedBuffer.h"

std::vector<__int8> node_input;

void SystolicDataSetup(int mode, int input_length) { // B x 256
	srand(time(0));
	//needs to be in 2d array
	//for (int i = 0; i < input_length * MATRIX_SIZE; i++) {
	//	int tmp = (rand() % 256) - (256 / 2);
	//	if (tmp != 0) {
	//		node_input.push_back(tmp); // related to __int8 range, not the matrix size
	//	}
	//	else {
	//		node_input.push_back(1);
	//	}
	//	
	//}

	//test code!!!
	for (int i = 0; i < 1 * MATRIX_SIZE; i++) {
		node_input.push_back(i + 1); // related to __int8 range, not the matrix size
	}
}