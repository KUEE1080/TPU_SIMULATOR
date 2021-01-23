#include "MatrixMultiplyUnit.h"

std::vector<Cell> Cells;
__int8 InputBuffer[MATRIX_SIZE][MATRIX_SIZE * 2];
__int32 Accumulator[ACCUMULATOR_SIZE][MATRIX_SIZE];

//Control logic
__int32 buffer_accumulator[MATRIX_SIZE] = { 0, }; // MMU에서 Accumulator로 넘어가기 전에 잠깐 대기하는 값들
int accumulator_index[ACCUMULATOR_SIZE] = { 0, }; //Bx256일때 각 entry안에 있는 256개의 32bit 저장 위치를 트래킹하기 위한 인덱스 저장소
int ibuf_index = 0; // index bit to implement the InputBuffer to be like a queue
int input_matrix_col = -1;


Cell::Cell(__int8 _weight) :
	//weight(_weight), input(0), partial_sum(0), tmp_in(0), tmp_psum(0), rightCell(NULL), downCell(NULL) {};
	weight(_weight), input(0), partial_sum(0), tmp_in(0), tmp_psum(0), rightval(NULL), downval(NULL) {};

void Cell::mac() { // this operation is done in one cycle
	//일단 weight은 preload되어있고 input은 다 전달받았어야한다.
	//[개발사항] 만약 mac이 정상적으로 이렇게 곱해지는 활동이 감지되면 (dataflow만 하는 경우 X), utilize 된거라고 볼 수 있다.
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

	//[주의사항] node_input의 사이즈는 Bx256이다. 256은 무조건 고정이다

	//Step1: input buffer initialization
	for (int i = 0; i < MATRIX_SIZE; i++) { for (int j = 0; j < MATRIX_SIZE * 2; j++) { InputBuffer[i][j] = 0; } }

	//Step2: accumulator initialization
	for (int i = 0; i < ACCUMULATOR_SIZE; i++) { for (int j = 0; j < MATRIX_SIZE; j++) { Accumulator[i][j] = 0; } }

	//Step3: MMU initialization (loading weights & interconnecting cells)
	//for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) { Cells.push_back(Cell((rand() % 256) - (256 / 2))); }
	//transposed the weight matrix!!!

	Cells.push_back(11); Cells.push_back(15); Cells.push_back(19); Cells.push_back(23);
	Cells.push_back(12); Cells.push_back(16); Cells.push_back(20); Cells.push_back(24);
	Cells.push_back(13); Cells.push_back(17); Cells.push_back(21); Cells.push_back(25);
	Cells.push_back(14); Cells.push_back(18); Cells.push_back(22); Cells.push_back(26);

	for (int i = 0; i < MATRIX_SIZE - 1; i++) {
		for (int j = 0; j < MATRIX_SIZE - 1; j++) {
			Cells[i * MATRIX_SIZE + j].interconnect(&Cells[i * MATRIX_SIZE + (j + 1)].tmp_in, &Cells[(i + 1) * MATRIX_SIZE + j].tmp_psum);
			// (__int16* rval, __int32* dval)
		}
	}

	//[개발사항] MMU랑 accumulator를 이어줘야하는데, 아직 accumulator의 구조를 제대로 몰라서 못하고 있는 실정이다.
	//일단 그냥 0번째 엔트리에 다 몰아넣는 구조로 가정하고 작성해보자.
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

int MMU_run() { // operation matrix multiply unit done in one cycle
	//new code
	if (ibuf_index == MATRIX_SIZE * 2) { // preventing overflow
		return COMPLETE;
	}
	else { //propagating inputbuffer to MMU
		for (int i = 0; i < MATRIX_SIZE; i++) {
			Cells[i * MATRIX_SIZE].input = InputBuffer[i][ibuf_index];
			//Cells[i * MATRIX_SIZE].partial_sum = 0; -- bug
		}
		ibuf_index++;
	}
	int dd = Cells[0].input;

	//[개발 사항] MMU보다 큰 행렬일 경우, 이걸 어캐할지는 생각을 해봐야한다.
	//애초에 근데 MMU보다 작은 행렬은 ISA에서 지원하지 않는다.

	// 곱한 다음 이동시키고~~~ 일단 이렇게 짰다.

	//new code
	for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) { Cells[i].mac(); }
	for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) { Cells[i].propagate(); }

	//[개발사항] accumulator_index를 계속 동적으로 키워나가야 한다. (1x256만 존재하는게 아니고 Bx256도 존재가능하기 때문에)

	//일단 1x256인 상황만 고려하는중이다
	//Bx256인 경우는 Accumulator[i][0]이러면 안되고, 저 2번째 인덱스가 동적으로 바꿔야한다. 해당 인덱스를 트래킹하는 마이크로아키텍쳐가 요구될것으로 예상
	for (int i = 0; i < MATRIX_SIZE; i++) {
		if (buffer_accumulator[i] != 0) {
			Accumulator[i][0] = buffer_accumulator[i];
		}
	}
	int d1 = buffer_accumulator[0];
	return IN_PROGRESS;
}