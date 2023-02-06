#include <iostream>
#include <vector>
#include "Algorithm.h"

using namespace std;


int main()
{
	Algorithm algorithm(0.5, 0.001);
	
	vector<float> inputs1= {0, 0};
	vector<float> inputs2 = {0, 1};
	vector<float> inputs3 = {1, 0};
	vector<float> inputs4 = {1, 1};

	vector<vector<float>> inputs;
	inputs.push_back(inputs1);
	inputs.push_back(inputs2);
	inputs.push_back(inputs3);
	inputs.push_back(inputs4);

	vector<float> desiredOutput1 = {0};
	vector<float> desiredOutput2 = {1};
	vector<float> desiredOutput3 = {1};
	vector<float> desiredOutput4 = {0};

	vector<vector<float>> desiredOutput;
	desiredOutput.push_back(desiredOutput1);
	desiredOutput.push_back(desiredOutput2);
	desiredOutput.push_back(desiredOutput3);
	desiredOutput.push_back(desiredOutput4);
	algorithm.startAlgorithm(2, 1, inputs, desiredOutput);

	return 0;
}
