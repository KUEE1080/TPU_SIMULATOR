#include "MatrixMultiplyUnit.h"

std::vector<Cell> Cells;
__int8 InputBuffer[MATRIX_SIZE][MATRIX_SIZE * 2];
__int32 Accumulator[ACCUMULATOR_SIZE][MATRIX_SIZE];

//Control logic
__int32 buffer_accumulator[MATRIX_SIZE] = { 0, }; // MMU���� Accumulator�� �Ѿ�� ���� ��� ����ϴ� ����
int accumulator_index[ACCUMULATOR_SIZE] = { 0, }; //Bx256�϶� �� entry�ȿ� �ִ� 256���� 32bit ���� ��ġ�� Ʈ��ŷ�ϱ� ���� �ε��� �����
int ibuf_index = 0; // index bit to implement the InputBuffer to be like a queue
int input_matrix_col = -1;
int count_idle_cell = 0;


Cell::Cell(__int8 _weight) :
	weight(_weight), input(0), partial_sum(0), tmp_in(0), tmp_psum(0), rightval(NULL), downval(NULL) {};

void Cell::mac() { // this operation is done in one cycle
	//�ϴ� weight�� preload�Ǿ��ְ� input�� �� ���޹޾Ҿ���Ѵ�.
	if (this->input == 0) { count_idle_cell++; } // used for simulation analysis
	this->partial_sum += (this->input) * (this->weight);
}

void Cell::propagate() {

	if (this->downval != NULL) {
		*downval = this->partial_sum;
		this->partial_sum = this->tmp_psum;
	}
	else { // end of the MMU
		this->partial_sum = this->tmp_psum;
	}

	if (this->rightval != NULL) {
		*rightval = this->input;
		this->input = this->tmp_in;
	}
	else { // end of the MMU
		this->input = this->tmp_in;
	}
}

void Cell::interconnect(__int8* rval, __int32* dval) {
	this->rightval = rval;
	this->downval = dval;
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
	for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) { Cells.push_back(preloadWeight()); }
	//transposed the weight matrix!!!

	/*Cells.push_back(11); Cells.push_back(15); Cells.push_back(19); Cells.push_back(23);
	Cells.push_back(12); Cells.push_back(16); Cells.push_back(20); Cells.push_back(24);
	Cells.push_back(13); Cells.push_back(17); Cells.push_back(21); Cells.push_back(25);
	Cells.push_back(14); Cells.push_back(18); Cells.push_back(22); Cells.push_back(26);*/

	for (int i = 0; i < MATRIX_SIZE - 1; i++) {
		for (int j = 0; j < MATRIX_SIZE - 1; j++) {
			Cells[i * MATRIX_SIZE + j].interconnect(&Cells[i * MATRIX_SIZE + (j + 1)].tmp_in, &Cells[(i + 1) * MATRIX_SIZE + j].tmp_psum);
			// (__int16* rval, __int32* dval)
		}
	}

	for (int i = 0; i < MATRIX_SIZE - 1; i++) {
		Cells[MATRIX_SIZE * (MATRIX_SIZE - 1) + i].interconnect(&Cells[MATRIX_SIZE * (MATRIX_SIZE - 1) + i + 1].tmp_in, 
			&buffer_accumulator[i]);
	}
	Cells[MATRIX_SIZE * MATRIX_SIZE - 1].interconnect(NULL, &buffer_accumulator[MATRIX_SIZE - 1]);

	for (int i = 0; i < MATRIX_SIZE - 1; i++) {
		Cells[MATRIX_SIZE * i + (MATRIX_SIZE - 1)].interconnect(NULL, &Cells[MATRIX_SIZE * (i + 1) + (MATRIX_SIZE - 1)].tmp_psum);
	}

	//Step5: Systolic Data Setup
	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < input_size; j++) {
			InputBuffer[i][i + j] = node_input[i * input_size + j];
		}
	}

	int a = InputBuffer[1][0];

}

int MMU_run() { // operation matrix multiply unit done in ONE CYCLE
	if (ibuf_index >= MATRIX_SIZE * 2) { // preventing overflow
		//MMU�ȿ� partial sum�ϰ� input�� 0���� �����Ǵ� ��� => literally doing nothing �� ���, return COMPLETE �ϱ�!!!
		if (count_idle_cell == MATRIX_SIZE * MATRIX_SIZE) { return COMPLETE; }
		
	}
	else { //propagating inputbuffer to MMU
		for (int i = 0; i < MATRIX_SIZE; i++) {
			Cells[i * MATRIX_SIZE].input = InputBuffer[i][ibuf_index];
			//Cells[i * MATRIX_SIZE].partial_sum = 0; -- bug
		}
		ibuf_index++;
	}
	
	int dd = Cells[0].input;

	//[���� ����] MMU���� ū ����� ���, �̰� ��ĳ������ ������ �غ����Ѵ�.

	//new code
	count_idle_cell = 0; // resetting the variable for the new cycle
	for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) { Cells[i].mac(); }
	for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) { Cells[i].propagate(); }

	//��Ȯ�� ���ϸ� �� 4096�� � ������� allocate������ϴ°�??? �������� ��ɾ �����ٰ� �ߴµ�... �ϴ� ��ɾ ��ĳ ���ܸԾ������� �𸥴�.
	for (int i = 0; i < MATRIX_SIZE; i++) {
		if (buffer_accumulator[i] != 0) {
			Accumulator[i][accumulator_index[i]] = buffer_accumulator[i];
			accumulator_index[i]++;
		}
	}
	int d1 = Accumulator[0][0];

	Analysis_incrementIdleRate(count_idle_cell);
	Analysis_sendCycleDone();
	
	return IN_PROGRESS;
}