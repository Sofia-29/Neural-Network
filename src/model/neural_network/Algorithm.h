#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Neuron.h"
#include "NeuronLayer.h"

using namespace std;

class Algorithm
{
public:
	Algorithm();
	Algorithm(float, float);
	~Algorithm();

	void startAlgorithm(int, int, vector<vector<float>>*, vector<vector<float>>*);
	void givePredictions(vector<float>, vector<float>);

	void neuronLayerActivation(vector<Neuron*>*, vector<float>, vector<float>);
	void neuronLayerWeightTraining(int, vector<Neuron*>*, vector<float>, vector<float>);

private:
	float threshold;
	float learningRate;
	int sumOfSquaredErrors;

	vector<vector<float>> *actualOutputs;
	vector<vector<float>> *desiredOutputs;
	vector<vector<float>> *previousWeights;
	vector<float> *previousLayerErrors;
	vector<float> *trainingErrors;
};

Algorithm::Algorithm()
{
	threshold = 0.0;
	learningRate = 0.0;
	sumOfSquaredErrors = 0;
	actualOutputs = new vector<vector<float>>();
	desiredOutputs = new vector<vector<float>>();
	previousWeights = new vector<vector<float>>();
	previousLayerErrors = new vector<float>();
	trainingErrors = new vector<float>();
}

Algorithm::Algorithm(float threshold, float learningRate)
{
	this->threshold = threshold;
	this->learningRate = learningRate;
	sumOfSquaredErrors = 0;
	actualOutputs = new vector<vector<float>>();
	desiredOutputs = new vector<vector<float>>();
	previousWeights = new vector<vector<float>>();
	previousLayerErrors = new vector<float>();
	trainingErrors = new vector<float>();
}

void Algorithm::startAlgorithm(int numberOfInputsNeurons, int numberOfOutputNeurons,
							   vector<vector<float>>*inputs, vector<vector<float>>* desiredOutputs)
{
	
}

void Algorithm::neuronLayerActivation(vector<Neuron*>* neuronLayer, vector<float> input, vector<float> output)
{
}

void Algorithm::neuronLayerWeightTraining(int desiredOutputIndex, vector<Neuron*>* neuronLayer, vector<float> input, vector<float> output)
{
}