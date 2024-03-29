#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <chrono>

#include "Neuron.h"
#include "NeuronLayer.h"
#include "Functions.h"
#include "FileManager.h"

using namespace std;

class Algorithm
{
public:
	Algorithm();
	Algorithm(float, float);
	~Algorithm();

	void startTraining(int, int, vector<vector<float>>, vector<vector<float>>);
	void testNeuralNetwork(string, int, int, vector<vector<float>>, vector<vector<float>>);
	void givePredictions(vector<Neuron *> *, vector<vector<float>>&, vector<float>);
	int interpretOutput(float);

	void loadWeights(NeuronLayer *inputLayer);
	void feedForward(vector<Neuron *> *);
	void backpropagation(vector<Neuron *> *, vector<float>);
	void neuronLayerActivation(vector<Neuron *> *, vector<float>);
	void neuronLayerWeightCorrection(vector<Neuron *> *, vector<float>, vector<vector<float>> &, vector<float> &);
	float getNeuronErrorGradient(string, vector<float>, vector<float>, int);

private:
	float threshold;
	float learningRate;
	int correct_predictions;
	int outputLayerErrors;

	float sumOfSquaredErrors;

	Functions *functions;
	FileManager *fileManager;
	vector<vector<float>> actualOutputs;
	vector<vector<float>> layerInputs;

	vector<float> previousLayerErrors;
	vector<vector<float>> previousWeights;
	vector<float> trainingErrors;
};

Algorithm::Algorithm()
{
	threshold = 0.0;
	learningRate = 0.0;
	correct_predictions = 0;
	outputLayerErrors = 0;
	functions = new Functions();
	fileManager = new FileManager();
}

Algorithm::Algorithm(float threshold, float learningRate)
{
	this->threshold = threshold;
	this->learningRate = learningRate;
	correct_predictions = 0;
	outputLayerErrors = 0;
	functions = new Functions();
	fileManager = new FileManager();
}

Algorithm::~Algorithm()
{
	delete this->functions;
	delete this->fileManager;
}

void Algorithm::startTraining(int numberOfInputsNeurons, int numberOfOutputNeurons,
							  vector<vector<float>> inputs, vector<vector<float>> desiredOutputs)
{
	int dataSize = inputs.size();
	//	int numberOfHiddenNeurons = numberOfInputsNeurons * 2 + 1;

	int numberOfHiddenNeurons = numberOfInputsNeurons;
	int iterations = 0;
	float accuracy = 0.0;
	std::chrono::time_point<std::chrono::system_clock> instanteInicial, instanteFinal;

	NeuronLayer *inputLayer = new NeuronLayer(numberOfInputsNeurons, "inputLayer", 0);
	NeuronLayer *hiddenLayer = new NeuronLayer(numberOfHiddenNeurons, "hiddenLayer", numberOfInputsNeurons);
	NeuronLayer *outputLayer = new NeuronLayer(numberOfOutputNeurons, "outputLayer", numberOfHiddenNeurons);

	inputLayer->connectNeurons(hiddenLayer);
	hiddenLayer->connectNeurons(outputLayer);
	instanteInicial = std::chrono::system_clock::now();
	do
	{
		sumOfSquaredErrors = 0.0;
		correct_predictions = 0;
		accuracy = 0;
		for (int indexData = 0; indexData < dataSize; indexData++)
		{
			this->layerInputs.push_back(inputs[indexData]);
			this->feedForward(hiddenLayer);
			this->backpropagation(outputLayer, desiredOutputs[indexData]);
		}
		iterations++;
		accuracy = (float)correct_predictions / (float)dataSize;
		this->trainingErrors.push_back(sumOfSquaredErrors);
	} while (sumOfSquaredErrors > 0.001 || accuracy < 0.95);

	instanteFinal = std::chrono::system_clock::now();
	std::chrono::duration<double> segundos = instanteFinal - instanteInicial;
	trainingErrors.push_back((float)segundos.count());
	fileManager->saveCSV(trainingErrors);
	fileManager->createWeightsFile(inputLayer);
	delete inputLayer;
	delete hiddenLayer;
	delete outputLayer;
}

void Algorithm::testNeuralNetwork(string fileName, int numberOfInputsNeurons, int numberOfOutputNeurons,
								  vector<vector<float>> inputs, vector<vector<float>> desiredOutputs)
{
	int dataSize = inputs.size();

	int numberOfHiddenNeurons = numberOfInputsNeurons;

	vector<vector<float>> results;
	NeuronLayer *inputLayer = new NeuronLayer(numberOfInputsNeurons, "inputLayer", 0);
	NeuronLayer *hiddenLayer = new NeuronLayer(numberOfHiddenNeurons, "hiddenLayer", numberOfInputsNeurons);
	NeuronLayer *outputLayer = new NeuronLayer(numberOfOutputNeurons, "outputLayer", numberOfHiddenNeurons);
	inputLayer->connectNeurons(hiddenLayer);
	hiddenLayer->connectNeurons(outputLayer);
	loadWeights(inputLayer);

	for (int indexData = 0; indexData < dataSize; indexData++)
	{
		this->layerInputs.push_back(inputs[indexData]);
		givePredictions(hiddenLayer, results, desiredOutputs[indexData]);
	}

	fileManager->saveResults(results, fileName);
}

void Algorithm::givePredictions(vector<Neuron *> *neuronLayer,vector<vector<float>>& results, vector<float> output)
{
	while (!neuronLayer->empty())
	{
		vector<float> inputNextLayer;
		vector<float> input = this->layerInputs.back();

		int size = neuronLayer->size();
		for (int neuronIndex = 0; neuronIndex < size; neuronIndex++)
		{
			Neuron *neuron = neuronLayer->at(neuronIndex);
			float value = neuron->activationFunction(input);
			if (neuron->getRole() == "outputLayer"){
				int prediction = interpretOutput(value);
				value = prediction;
			}
			inputNextLayer.push_back(value); 
		}
		this->layerInputs.erase(this->layerInputs.begin() + this->layerInputs.size() - 1);
		this->layerInputs.push_back(inputNextLayer);
		if(neuronLayer->at(0)->getRole() == "outputLayer"){
			results.push_back(inputNextLayer);
		}
		neuronLayer = neuronLayer->at(0)->getNextLayer();
	}
}

void Algorithm::loadWeights(NeuronLayer *inputLayer)
{
	vector<Neuron *> *neuronLayer = inputLayer->at(0)->getNextLayer();
	vector<vector<float>> *weights = new vector<vector<float>>();
	fileManager->readResultsWeightsCSV(weights);
	int weightIndex = 0;
	do 
	{
		for (int index = 0; index < neuronLayer->size(); index++)
		{
			Neuron *neuron = neuronLayer->at(index);
			vector<float> weight = weights->at(weightIndex);
			float bias = weight[weight.size() - 1];
			weight.erase(weight.begin() + weight.size() - 1);
			neuron->setWeights(weight);
			neuron->setBias(bias);
			weightIndex++;
		}
		neuronLayer = neuronLayer->at(0)->getNextLayer();
	} while(!neuronLayer->empty());
}

void Algorithm::feedForward(vector<Neuron *> *neuronLayer)
{
	// All data goes throught the layers, starting from the input layer and ending in the output layer.
	while (!neuronLayer->empty())
	{
		this->neuronLayerActivation(neuronLayer, this->layerInputs.back());
		neuronLayer = neuronLayer->at(0)->getNextLayer();
	}
}

void Algorithm::neuronLayerActivation(vector<Neuron *> *neuronLayer, vector<float> input)
{
	vector<float> outputValues;
	vector<float> inputNextLayer;
	int size = neuronLayer->size();
	for (int neuronIndex = 0; neuronIndex < size; neuronIndex++)
	{
		Neuron *neuron = neuronLayer->at(neuronIndex);
		float value = neuron->activationFunction(input);
		inputNextLayer.push_back(value); // The output of a layer is the input of the next layer
		outputValues.push_back(value);
	}
	this->actualOutputs.push_back(outputValues);
	if (neuronLayer->front()->getRole() != "outputLayer")
	{ // We don't need to store the output of Output layer as input of next layer
		this->layerInputs.push_back(inputNextLayer);
	}
}

void Algorithm::backpropagation(vector<Neuron *> *outputLayer, vector<float> desiredOutput)
{
	vector<Neuron *> *neuronLayer = outputLayer;
	vector<float> previousLayerErrors;
	vector<vector<float>> previousWeights;
	this->outputLayerErrors = 0;
	// Cycle to travers all layers from last one to first one.
	while (neuronLayer->at(0)->getRole() != "inputLayer")
	{
		vector<vector<float>> layerWeights;
		vector<float> layerErrors;
		this->neuronLayerWeightCorrection(neuronLayer, desiredOutput, layerWeights, layerErrors);
		neuronLayer = neuronLayer->at(0)->getPreviousLayer();
		this->previousLayerErrors = layerErrors;
		this->previousWeights = layerWeights;
	}

	if (this->outputLayerErrors == 0)
	{
		this->correct_predictions++;
	}
}

void Algorithm::neuronLayerWeightCorrection(vector<Neuron *> *neuronLayer, vector<float> desiredOutput,
											vector<vector<float>> &layerWeights, vector<float> &layerErrors)
{
	int size = neuronLayer->size();
	string neuronLayerRole = neuronLayer->at(0)->getRole();
	vector<float> output = this->actualOutputs.back();
	vector<float> input = this->layerInputs.back();
	for (int neuronIndex = 0; neuronIndex < size; neuronIndex++)
	{
		Neuron *neuron = neuronLayer->at(neuronIndex);
		layerWeights.push_back(neuron->getWeights());
		float errorGradient = this->getNeuronErrorGradient(neuronLayerRole, desiredOutput, output, neuronIndex);
		vector<float> newWeights = neuron->weightsCorrection(learningRate, errorGradient, input);
		neuron->setWeights(newWeights);
		layerErrors.push_back(errorGradient);
	}
	this->layerInputs.erase(this->layerInputs.begin() + this->layerInputs.size() - 1);
	this->actualOutputs.erase(this->actualOutputs.begin() + this->actualOutputs.size() - 1);
}

float Algorithm::getNeuronErrorGradient(string neuronLayerRole, vector<float> desiredOutput,
										vector<float> output, int neuronIndex)
{
	float errorGradient = 0.0;
	if (neuronLayerRole == "outputLayer")
	{
		float error = this->functions->getError(desiredOutput[neuronIndex], output[neuronIndex]);
		sumOfSquaredErrors += pow(error, 2);
		errorGradient = this->functions->calculateErrorForOutputLayer(desiredOutput[neuronIndex], output[neuronIndex], error);
		int interpretedError = pow(this->functions->getError(desiredOutput[neuronIndex], this->interpretOutput(output[neuronIndex])), 2);
		this->outputLayerErrors += interpretedError;
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