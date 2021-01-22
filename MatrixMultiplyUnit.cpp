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
	//�ϴ� weight�� preload�Ǿ��ְ� input�� �� ���޹޾Ҿ���Ѵ�.
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

	//[���ǻ���] node_input�� ������� Bx256�̴�. 256�� ������ �����̴�

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

	//[���߻���] MMU�� accumulator�� �̾�����ϴµ�, ���� accumulator�� ������ ����� ���� ���ϰ� �ִ� �����̴�.
	//�ϴ� �׳� 0��° ��Ʈ���� �� ���Ƴִ� ������ �����ϰ� �ۼ��غ���.
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
	//[���� ����] MMU���� ū ����� ���, �̰� ��ĳ������ ������ �غ����Ѵ�.
	//���ʿ� �ٵ� MMU���� ���� ����� ISA���� �������� �ʴ´�.

	// ���� ���� �̵���Ű��~~~ �ϴ� �̷��� ®��.

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