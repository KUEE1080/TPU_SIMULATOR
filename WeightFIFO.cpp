//[ASSUMPTION!]: the memory situations are ideal, so most of the hardware constraints are not considered for simplicity

#include "WeightFIFO.h"

__int8 preloadWeight() {
	assigned_weight.push_back((rand() % 256) - (256 / 2));
	return assigned_weight.back();
}