#include "MatrixMultiplyUnit.h"

std::vector<Cell> Cells;
__int8 InputBuffer[MATRIX_SIZE][MATRIX_SIZE * 2];
__int32 Accumulator[ACCUMULATOR_SIZE][MATRIX_SIZE];

int ibuf_index = 0; // index bit to implement the InputBuffer to be like a queue
int input_matrix_col = -1;

Cell::Cell(__int8 _weight) : 
	//use(false), phase(IDLE), weight(_weight), input(0), partial_sum(0), rightCell(NULL), downCell(NULL) {};
	weight(_weight), input(0), partial_sum(0), 
	tmp_in(0), tmp_psum(0), rightCell(NULL), downCell(NULL) {};

void Cell::mac() { // this operation is done in one cycle
	//일단 weight은 preload되어있고 input은 다 전달받았어야한다.
	this->partial_sum += (this->input) * (this->weight);
}

void Cell::propagate() {
	if (this->downCell != NULL) {
		this->downCell->tmp_psum = this->partial_sum;
		this->partial_sum = this->tmp_psum;
	}
	if (this->rightCell != NULL) {
		this->rightCell->tmp_in = this->input;
		this->input = this->tmp_in;
	}
}

void Cell::interconnect(Cell* R_Cell, Cell* D_Cell) {
	this->rightCell = R_Cell;
	this->downCell = D_Cell;
}


//----------------------------------------------------------------------------------------------------------------------


void MMU_initialize(int mode, int input_size) {
	input_matrix_col = input_size;
	
	srand(time(0));

	//[주의사항] node_input의 사이즈는 Bx256이다. 256은 무조건 고정이다

	//Step1: input buffer initialization
	for (int i = 0; i < MATRIX_SIZE; i++) { for (int j = 0; j < MATRIX_SIZE * 2; j++) { InputBuffer[i][j] = 0; } }

	//Step2: accumulator initialization
	for (int i = 0; i < ACCUMULATOR_SIZE; i++) { for (int j = 0; j < MATRIX_SIZE; j++) { Accumulator[i][j] = 0; } }

	//Step3: MMU initialization (loading weights & interconnecting cells)
	for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) { Cells.push_back(Cell((rand() % MATRIX_SIZE) - (MATRIX_SIZE / 2))); }
	for (int i = 0; i < MATRIX_SIZE - 1; i++) {
		for (int j = 0; j < MATRIX_SIZE - 1; j++) {
			Cells[i * MATRIX_SIZE + j].interconnect(&Cells[j + 1], &Cells[(i + 1) * MATRIX_SIZE + j]); // (Cell* R_Cell, Cell* D_Cell)
		}
	}

	//[개발사항] MMU랑 accumulator를 이어줘야하는데, 아직 accumulator의 구조를 제대로 몰라서 못하고 있는 실정이다.
	//일단 그냥 0번째 엔트리에 다 몰아넣는 구조로 가정하고 작성해보자.
	//Step4: interconnecting MMU & Accumulators
	/*for (int i = 0; i < MATRIX_SIZE; i++) {
		Cells[MATRIX_SIZE * (MATRIX_SIZE - 1) + i].interconnect();
	}*/

	//Step5: Systolic Data Setup
	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < input_size; j++) {
			InputBuffer[i][i + j] = node_input[i * input_size + j];
		}
	}

	//int a = InputBuffer[1][0];

}

int MMU_run() { // operation matrix multiply unit done in one cycle
	//new code
	if (ibuf_index == input_matrix_col) {
		//return COMPLETE;
	}
	else { //propagating inputbuffer to MMU
		
	}
	//testing zone
	for (int i = 0; i < MATRIX_SIZE; i++) {
		Cells[i * MATRIX_SIZE].input = InputBuffer[i][ibuf_index];
		Cells[i * MATRIX_SIZE].partial_sum = 0;
		//Cells[i * MATRIX_SIZE].phase = INPUT_LOAD;
	}
	ibuf_index++;
	//[개발 사항] MMU보다 큰 행렬일 경우, 이걸 어캐할지는 생각을 해봐야한다.
	//애초에 근데 MMU보다 작은 행렬은 ISA에서 지원하지 않는다.

	// 곱한 다음 이동시키고~~~ 일단 이렇게 짰다.

	//new code
	for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {
		Cells[i].mac();
	}
	for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {
		Cells[i].propagate();
	}

	int d1 = Cells[1].partial_sum;

	//int d = ProcessElements[320].input;
	return IN_PROGRESS;
}