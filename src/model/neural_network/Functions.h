#pragma once
#include <vector>
#include <math.h>

using namespace std;

class Functions
{
public:
	Functions();
	~Functions();

	float sigmoidFunction(float);
	float sigmoidDerivatedFunction(float);
	float getError(float, float);
	float calculateErrorForOutputLayer(float, float);
	float calculateErrorForHiddenLayer(float, vector<float>, vector<vector<float>>, int);
};

Functions::Functions() {}

Functions::~Functions() {}

inline float Functions::sigmoidFunction(float sum)
{
	return 1 / (1 + exp(-(sum)));
}

inline float Functions::sigmoidDerivatedFunction(float output)
{
	return output * (1 - output);
}

inline float Functions::getError(float desiredOutput, float actualOutput)
{
	return desiredOutput - actualOutput;
}

inline float Functions::calculateErrorForOutputLayer(float desiredOutput, float actualOutput)
{
	float errorGradient = sigmoidDerivatedFunction(actualOutput) * getError(desiredOutput, actualOutput);
	return errorGradient;
}

float Functions::calculateErrorForHiddenLayer(float actualOutput, vector<float> errorGradient,
											  vector<vector<float>> previousWeight, int connection)
{
	float errorGradientHiddenLayer = actualOutput * (1 - actualOutput);
	// float sum = 0.0;
	for (int index = 0; index < errorGradient.size(); index++)
	{
		errorGradientHiddenLayer *= previousWeight[index][connection] * errorGradient[index];
	}
	// errorGradientHiddenLayer *= sum;
	return errorGradientHiddenLayer;
}