#pragma once

#include <stdlib.h>
#include <time.h>
#include <vector>
#include "UnifiedBuffer.h"

#define MATRIX_SIZE 256
#define ACCUMULATOR_SIZE 4096

#define IN_PROGRESS 0
#define COMPLETE 1

#define IDLE -1
#define INPUT_LOAD 0
#define MULTIPLY 1
#define PROPAGATE 2

class ProcessElement {
private:
	__int8 weight; // weight stationary
	__int32 partial_sum;

public:
	bool use;
	__int8 input; // temporary storage to save the input
	ProcessElement(int _weight);
	__int32 multiply();
	/*__int8 getInput();
	__int32 getPartialSum();*/

};

//new class
class Cell {
public:
	Cell(__int8 _weight);
	
	//bool use;

	//__int8 phase;
	__int8 tmp_in;
	__int16 input;
	__int32 tmp_psum;

	__int8 weight;
	
	__int32 partial_sum;

	Cell* rightCell;
	Cell* downCell;

	void interconnect(Cell* R_Cell, Cell* D_Cell);
	void mac();
	void propagate();

};

void MMU_initialize(int mode, int input_size);
//void MMU_assignWeight(int weight_size);
int MMU_run();

extern __int32 Accumulator[ACCUMULATOR_SIZE][MATRIX_SIZE];