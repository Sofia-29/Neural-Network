#include <iostream>
#include "Algorithm.h"

int main()
{
	Algorithm algorithm(0.5, 0.5);

	vector<float> in1 = { 1, 1 };
	vector<float> in2 = { 1, 0 };
	vector<float> in3 = { 0, 1 };
	vector<float> in4 = { 0, 0 };

	vector<vector<float>> inputs = { in1, in2, in3, in4 };

	vector<float> ds1 = { 0 };
	vector<float> ds2 = { 1 };
	vector<float> ds3 = { 1 };
	vector<float> ds4 = { 0 };

	vector<vector<float>> desiredOutputs = { ds1, ds2, ds3, ds4 };

	algorithm.startAlgorithm(2, 1, inputs, desiredOutputs);

	return 0;
}
