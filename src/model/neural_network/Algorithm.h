#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Neuron.h"
#include "NeuronLayer.h"
#include "Functions.h"
#include <chrono>

using namespace std;

class Algorithm
{
public:
	Algorithm();
	Algorithm(float, float);
	~Algorithm();

	void startAlgorithm(int, int, vector<vector<float>>, vector<vector<float>>);
	void givePredictions(vector<float>, vector<float>);
	int interpretOutput(float);

	void feedForward(vector<float>, vector<Neuron *> *);
	void backpropagation(vector<Neuron *> *, vector<float>);
	void neuronLayerActivation(vector<Neuron *> *, vector<float>, vector<float> &);
	void neuronLayerWeightCorrection(vector<Neuron *> *, vector<float>, vector<vector<float>> &, vector<float> &);
	float getNeuronErrorGradient(string, vector<float>, vector<float>, int);

private:
	float threshold;
	float learningRate;
	float sumOfSquaredErrors;

	Functions* functions;
	vector<vector<float>>* actualOutputs;
	vector<vector<float>>* layerInputs;
	vector<float>* trainingErrors;

	vector<float> previousLayerErrors;
	vector<vector<float>> previousWeights;
};

Algorithm::Algorithm()
{
	threshold = 0.0;
	learningRate = 0.0;
	sumOfSquaredErrors = 0.0;

	functions = new Functions();
	actualOutputs = new vector<vector<float>>();
	layerInputs = new vector<vector<float>>();
	trainingErrors = new vector<float>();
}

Algorithm::Algorithm(float threshold, float learningRate)
{
	this->threshold = threshold;
	this->learningRate = learningRate;
	sumOfSquaredErrors = 0.0;

	functions = new Functions();
	actualOutputs = new vector<vector<float>>();
	layerInputs = new vector<vector<float>>();
	trainingErrors = new vector<float>();
}

Algorithm::~Algorithm()
{
	delete this->functions;
	delete this->actualOutputs;
	delete this->layerInputs;
	delete this->trainingErrors;
}

void Algorithm::startAlgorithm(int numberOfInputsNeurons, int numberOfOutputNeurons,
							   vector<vector<float>> inputs, vector<vector<float>> desiredOutputs)
{
	int dataSize = inputs.size();
	//int numberOfHiddenNeurons = numberOfInputsNeurons * 2 + 1;
	
	int numberOfHiddenNeurons = 2;
	int iterations = 0;
	std::chrono::time_point<std::chrono::system_clock> instanteInicial, instanteFinal;
			
	vector<float> w1 = {0.5, 0.4};
	vector<float> w2 = {0.9, 1.0};
	vector<float> biasn1n2 = {0.8, -0.1};
	vector<float> biasn3n4 = {0.3};

	vector<vector<float>> wn1;
	wn1.push_back(w1);
	wn1.push_back(w2);

	vector<float> w3 = {-1.2, 1.1};

	vector<vector<float>> wn2;
	wn2.push_back(w3);

	// Fixed number of layers
	NeuronLayer* inputLayer = new NeuronLayer(numberOfInputsNeurons, "inputLayer", 0);
	NeuronLayer* hiddenLayer = new NeuronLayer(numberOfHiddenNeurons, "hiddenLayer", numberOfInputsNeurons, wn1, biasn1n2);
	NeuronLayer* outputLayer = new NeuronLayer(numberOfOutputNeurons, "outputLayer", numberOfHiddenNeurons, wn2, biasn3n4);

	// NeuronLayer *hiddenLayer = new NeuronLayer(numberOfHiddenNeurons, "hiddenLayer", numberOfInputsNeurons);
	// NeuronLayer *outputLayer = new NeuronLayer(numberOfOutputNeurons, "outputLayer", numberOfHiddenNeurons);

	inputLayer->connectNeurons(hiddenLayer);
	hiddenLayer->connectNeurons(outputLayer);
	instanteInicial = std::chrono::system_clock::now();
	do
	{
		sumOfSquaredErrors = 0.0;
		vector<float> layerInput;
		for (int indexData = 0; indexData < dataSize; indexData++)
		{
			layerInput = inputs[indexData];
			this->feedForward(layerInput, hiddenLayer);
			this->backpropagation(outputLayer, desiredOutputs[indexData]);
		}
		iterations++;
	} while (sumOfSquaredErrors > 0.001 );
	instanteFinal = std::chrono::system_clock::now();
	std::chrono::duration<double> segundos = instanteFinal - instanteInicial;
	cout << "SECONDS: " << segundos.count() << endl;
	cout << "Sum of squared errors: " << sumOfSquaredErrors << endl;
	cout << "Number of iterations: " << iterations << endl;
	delete inputLayer;
	delete hiddenLayer;
	delete outputLayer;
}

void Algorithm::feedForward(vector<float> layerInput, vector<Neuron*>* neuronLayer)
{
	// All data goes throught the layers, starting from the input layer and ending in the output layer.
	// index = 1 -> input layer, index = 2 -> hidden layer, index = 3 -> output layer.
	for (int indexLayer = 2; indexLayer <= 3; indexLayer++)
	{
		vector<float> inputNextLayer;
		this->layerInputs->push_back(layerInput);
		this->neuronLayerActivation(neuronLayer, layerInput, inputNextLayer);
		layerInput = inputNextLayer;
		neuronLayer = neuronLayer->at(0)->getNextLayer();
	}
}

void Algorithm::neuronLayerActivation(vector<Neuron*>* neuronLayer, vector<float> input,
									  vector<float> &inputNextLayer)
{
	vector<float> outputValues;
	int size = neuronLayer->size();
	for (int neuronIndex = 0; neuronIndex < size; neuronIndex++)
	{
		Neuron* neuron = neuronLayer->at(neuronIndex);
		float value = neuron->activationFunction(input);
		inputNextLayer.push_back(value); // The output of a layer is the input of the next layer
		outputValues.push_back(value);
	}
	this->actualOutputs->push_back(outputValues);
}

void Algorithm::backpropagation(vector<Neuron*>* outputLayer, vector<float> desiredOutput)
{
	vector<Neuron*>* neuronLayer = outputLayer;
	vector<float> previousLayerErrors;
	vector<vector<float>> previousWeights;
	// index = 3 -> output layer, index = 2 -> hidden layer, index = 1 -> input layer.
	// Cycle to travers all layers from last one to first one.
	for (int index = 3; index > 1; index--)
	{
		vector<vector<float>> layerWeights;
		vector<float> layerErrors;
		this->neuronLayerWeightCorrection(neuronLayer, desiredOutput, layerWeights, layerErrors);
		this->layerInputs->pop_back();
		this->actualOutputs->pop_back();
		neuronLayer = neuronLayer->at(0)->getPreviousLayer();
		this->previousLayerErrors = layerErrors;
		this->previousWeights = layerWeights;
	}
}

void Algorithm::neuronLayerWeightCorrection(vector<Neuron*>* neuronLayer, vector<float> desiredOutput,
											vector<vector<float>>& layerWeights, vector<float>& layerErrors)
{
	int size = neuronLayer->size();
	string neuronLayerRole = neuronLayer->at(0)->getRole();
	vector<float> output = this->actualOutputs->back();
	vector<float> input = this->layerInputs->back();
	for (int neuronIndex = 0; neuronIndex < size; neuronIndex++)
	{
		Neuron* neuron = neuronLayer->at(neuronIndex);
		layerWeights.push_back(neuron->getWeights());
		float errorGradient = this->getNeuronErrorGradient(neuronLayerRole, desiredOutput, output, neuronIndex);
		vector<float> newWeights = neuron->weightsCorrection(learningRate, errorGradient, input);
		neuron->setWeights(newWeights);
		layerErrors.push_back(errorGradient);
	}
}

float Algorithm::getNeuronErrorGradient(string neuronLayerRole, vector<float> desiredOutput,
										vector<float> output, int neuronIndex)
{
	float errorGradient = 0.0;
	if (neuronLayerRole == "outputLayer")
	{
		float error = this->functions->getError(desiredOutput[neuronIndex], output[neuronIndex]);
		this->sumOfSquaredErrors += pow(error, 2);
		errorGradient = this->functions->calculateErrorForOutputLayer(desiredOutput[neuronIndex], output[neuronIndex], error);
	}
	else
	{
		errorGradient = this->functions->calculateErrorForHiddenLayer(output[neuronIndex], this->previousLayerErrors,
																this->previousWeights, neuronIndex);
	}
	return errorGradient;
}

int Algorithm::interpretOutput(float outputValue)
{
	if (outputValue < threshold)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}