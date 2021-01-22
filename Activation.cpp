//[개발 사항] 현재 출력 결과물이 양자화된 데이터들의 곱으로 나온 결과로, 사실 시그모이드를 바로 때려박는게 맞는지는 모르겠다. 일단 ReLU 정도만 실제 코드로 구현을 해보자.

#include "Activation.h"

std::vector<__int32> activation_node;

void Activation_run(int mode) {
	if (mode == ReLU) {
		for (int i = 0; i < MATRIX_SIZE; i++) {
			//if (mmu_accumulator[i] > 0) { activation_node.push_back(mmu_accumulator[i]); }
			//else { activation_node.push_back(0); }
		}
	}
	else if (mode == Sigmoid) {  }
	else if (mode == tanh) {  }
}

