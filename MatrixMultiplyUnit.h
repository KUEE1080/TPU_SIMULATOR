#pragma once

#include <stdlib.h>
#include <time.h>
#include <vector>
#include "UnifiedBuffer.h"
#include "WeightFIFO.h"
#include "Analysis.h"

#define MATRIX_SIZE 256
#define ACCUMULATOR_SIZE 4096

#define IN_PROGRESS 0
#define COMPLETE 1


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
	//Cell�� �ƴ� __int32�� ���� ������, Cell�� �صθ� accumulator�� ������ �� ����� ��Ȥ���������� �׳� general �ϰ� __int32�� �״�.
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