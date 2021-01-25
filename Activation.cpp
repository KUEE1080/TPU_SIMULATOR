//[���� ����] ���� ��� ������� ����ȭ�� �����͵��� ������ ���� �����, ��� �ñ׸��̵带 �ٷ� �����ڴ°� �´����� �𸣰ڴ�. �ϴ� ReLU ������ ���� �ڵ�� ������ �غ���.

#include "Activation.h"

std::vector<int32_t> activation_node;

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

