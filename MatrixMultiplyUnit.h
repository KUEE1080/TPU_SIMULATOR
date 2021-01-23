#pragma once

#include <stdlib.h>
#include <time.h>
#include <vector>
#include "UnifiedBuffer.h"

#define MATRIX_SIZE 4
#define ACCUMULATOR_SIZE 4096

#define IN_PROGRESS 0
#define COMPLETE 1

#define IDLE -1
#define INPUT_LOAD 0
#define MULTIPLY 1
#define PROPAGATE 2


class Cell {
public:
	Cell(__int8 _weight);
	
	__int32 partial_sum;
	__int8 input;
	__int8 weight;
	__int8 tmp_in;
	__int32 tmp_psum;
	

	//Cell* rightCell;
	//Cell* downCell;

	//the value will be passed with the pointer variable below! Cell pointer variable is obsolete!
	//Cell이 아닌 __int32를 쓰는 이유는, Cell로 해두면 accumulator랑 연결할 때 상당히 곤혹스러워져서 그냥 general 하게 __int32로 뒀다.
	__int8* rightval;
	__int32* downval;

	//void interconnect(Cell* R_Cell, Cell* D_Cell);
	void interconnect(__int8* rval, __int32* dval);
	void mac();
	void propagate();

};

void MMU_initialize(int mode, int input_size);
//void MMU_assignWeight(int weight_size);
int MMU_run();

extern __int32 Accumulator[ACCUMULATOR_SIZE][MATRIX_SIZE];