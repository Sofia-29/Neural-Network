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
	float calculateErrorForOutputLayer(float, float, float);
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

inline float Functions::getError(float desiredOutput, float predictedOutput)
{
	return desiredOutput - predictedOutput;
}

inline float Functions::calculateErrorForOutputLayer(float desiredOutput, float predictedOutput, float error)
{
	float errorGradient = sigmoidDerivatedFunction(predictedOutput) * error;
	return errorGradient;
}

float Functions::calculateErrorForHiddenLayer(float predictedOutput, vector<float> errorGradient,
											  vector<vector<float>> previousWeight, int connection)
{
	float errorGradientHiddenLayer = predictedOutput * (1 - predictedOutput);
	float sum = 0.0;
	for (int index = 0; index < errorGradient.size(); index++)
	{
		sum += previousWeight[index][connection] * errorGradient[index];
	}
	errorGradientHiddenLayer *= sum;
	return errorGradientHiddenLayer;
}