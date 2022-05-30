#include <iostream>
#include <Python.h>
#include <numpy/arrayobject.h>
#include <vector>
#include "Neuron.h"
#include "NeuronLayer.h"
#include "FileManager.h"


using namespace std;


float threshold = 0.5;
float learningRate = 0.5;
int numberOfInputNeurons = 2;
int numberOfHiddenLayers = 1;
int numberOfNeuronsPerHiddenLayer = 2;
int numberOfOutputNeurons = 1;
int numberOfIterations = 2;
float sumOfSquaredErrors = 1.0;
vector<vector<float>> inputs;
vector<vector<float>> outputs;
vector<vector<float>> previousWeights;
vector<float> desiredOutputs;
vector<float> previousLayerErrors;
vector<float> trainingErrors;


void startAlgorithm();
int showGraphics();
void processActivationNeuronsLayers(vector<Neuron*>*, vector<float>, vector<float>&);
void processWeightTraining(vector<Neuron*>*, vector<float>, vector<float>, int);
int interpretOutput(float);


int main() {

	startAlgorithm();

	return 0;
}

void startAlgorithm()
{
	///Test data

	//Test weights
	/*vector<float> weightsNeuron1 = { 5.19372, 5.22143 };
	vector<float> weightsNeuron2 = { 3.22362, 3.2108 };
	vector<float> weightsNeuron3 = { 6.22006, -6.50913 };*/

	desiredOutputs = { 1, 0, 0, 0 };
	inputs = { { 1, 1 }, {0 , 1}, {1, 0}, {0, 0} };

	//Random weights
	NeuronLayer* nl1 = new NeuronLayer(2, "inputLayer", 0);
	NeuronLayer* nl2 = new NeuronLayer(2, "hiddenLayer", 2);
	NeuronLayer* nl3 = new NeuronLayer(1, "outputLayer", 2);

	//Test weights
	/*NeuronLayer* nl1 = new NeuronLayer(2, "inputLayer", 0);
	NeuronLayer* nl2 = new NeuronLayer(2, "hiddenLayer", weightsNeuron1, 1.97595, weightsNeuron2, 4.89495);
	NeuronLayer* nl3 = new NeuronLayer(1, "outputLayer", weightsNeuron3, 2.83256);*/
	nl1->connectNeurons(nl2);
	nl2->connectNeurons(nl3);


	cout << "------------------------------------------------------------" << endl; 
	cout << "INITIAL WEIGHTS:" << endl;
	cout << "------------------------------------------------------------" << endl;
	cout << nl1->toString() << endl;
	cout << nl2->toString() << endl;
	cout << nl3->toString() << endl;

	//Start iterations
	int count = 0;
	int size = inputs.size();
	while (sumOfSquaredErrors > 0.05) {
		sumOfSquaredErrors = 0.0;
		//process layers 
		for (int indexData = 0; indexData < size; indexData++) {
			vector<Neuron*>* neuronLayer = nl1;
			vector<vector<float>> inputsNextLayer;
			vector<float> input;
			string layerRole;
			bool backpropagation = false;

			while (neuronLayer->front()->getRole() != "inputLayer" || !backpropagation) {
				Neuron* neuron = neuronLayer->front();
				vector<float> output; //Output is going to be the input of the next layer
				layerRole = neuron->getRole();
				if (layerRole == "inputLayer" && !backpropagation) {
					inputsNextLayer.push_back(inputs[indexData]);
					neuronLayer = neuron->getNextLayer();
				}else {
					input = inputsNextLayer.back();
					if (!backpropagation) {
						processActivationNeuronsLayers(neuronLayer, input, output);
						if (neuron->getRole() == "outputLayer") {
							backpropagation = true;
						}
						else {
							inputsNextLayer.push_back(output);
							neuronLayer = neuron->getNextLayer();
						}
					} else {
						output = outputs.back();
						processWeightTraining(neuronLayer, input, output, indexData);
						neuronLayer = neuron->getPreviousLayer();
						inputsNextLayer.erase(inputsNextLayer.begin() + inputsNextLayer.size() - 1); //Delete back
						outputs.erase(outputs.begin() + outputs.size() - 1); //Delete back
					}
				}
			}
		}
		count++;
	}

	cout << "------------------------------------------------------------" << endl;
	cout << "FINAL WEIGHTS:" << endl;
	cout << "------------------------------------------------------------" << endl;
	cout << nl1->toString() << endl;
	cout << nl2->toString() << endl;
	cout << nl3->toString() << endl;
	cout << "------------------------------------------------------------" << endl;
	cout << "ITERATIONS:" << count << endl;
	cout << "------------------------------------------------------------" << endl;
	showGraphics();
}

void processActivationNeuronsLayers(vector<Neuron*>* neuronLayer, vector<float> input, vector<float>& output) {
	vector<float> outputValues;
	int size = neuronLayer->size();
	for (int neuronIndex = 0; neuronIndex < size; neuronIndex++){
		Neuron* neuron = neuronLayer->at(neuronIndex);
		float value = neuron->activationFunction(input);
		/*if(neuron.getRole() == "outputLayer")
			value = interpretOutput(value);*/
		output.push_back(value);
		outputValues.push_back(value);
	}
	outputs.push_back(outputValues);
}

void processWeightTraining(vector<Neuron*>* neuronLayer, vector<float> input, vector<float> output, int desiredOutputIndex) {
	vector<vector<float>> weights;
	vector<float> errors;
	string role = neuronLayer->at(0)->getRole();
	int size = neuronLayer->size();
	for (int neuronIndex = 0; neuronIndex < size; neuronIndex++) {
		Neuron* neuron = neuronLayer->at(neuronIndex);
		weights.push_back(neuron->getWeights());
		float errorGradient = 0.0;

		if (role == "outputLayer") {
			errorGradient = neuron->calculateErrorForOutputLayer(desiredOutputs[desiredOutputIndex], output[neuronIndex]);
			float error = neuron->getError(desiredOutputs[desiredOutputIndex], output[neuronIndex]);
			sumOfSquaredErrors += pow(error, 2);
			trainingErrors.push_back(sumOfSquaredErrors);
		}
		else {
			errorGradient = neuron->calculateErrorForHiddenLayer(output[neuronIndex], previousLayerErrors, previousWeights, neuronIndex);
		}

		vector<float> newWeights = neuron->updateWeight(learningRate, errorGradient, input);
		neuron->setWeights(newWeights);
		errors.push_back(errorGradient);
	}

	previousLayerErrors = errors;
	previousWeights = weights;
}

int showGraphics(){
	npy_intp errorsSize = trainingErrors.size();
	Py_Initialize();  
	import_array();
    FILE* file = fopen("Graphics.py", "r");
    float* errorsResults = new float[errorsSize];
    copy(trainingErrors.begin(), trainingErrors.end(), errorsResults);

    PyRun_SimpleFile(file, "Graphics.py");
    PyObject* modulo = PyImport_AddModule("__main__");
    PyObject* main = PyModule_GetDict(modulo);
    PyObject* graphicFunction = PyDict_GetItemString(main, "showGraphic");
    PyObject* args = PyArray_SimpleNewFromData(1, &errorsSize, NPY_FLOAT, errorsResults);
    PyObject_CallFunctionObjArgs(graphicFunction, args, NULL);

    Py_Finalize();

	return 0; 
}
//int interpretOutput(float outputValue) {
//	if (outputValue < threshold) 
//		return 0;
//	else 
//		return 1;
//}