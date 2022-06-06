#include <iostream>
#include <Python.h>
#include <numpy/arrayobject.h>
#include <vector>
#include <string>
#include <chrono>
#include "Neuron.h"
#include "NeuronLayer.h"
#include "FileManager.h"


using namespace std;


string fileName = "";
float threshold = 0.5;
float learningRate = 0.001;
int hits = 0;
int numberOfInputNeurons = 2;
int numberOfHiddenLayers = 1;
int numberOfNeuronsPerHiddenLayer = 2;
int numberOfOutputNeurons = 1;
int numberOfIterations = 2;
float sumOfSquaredErrors = 1;
vector<vector<float>>* inputs;
vector<vector<float>> outputs;
vector<vector<float>> previousWeights;
vector<vector<float>>* desiredOutputs;
vector<float> previousLayerErrors;
vector<float> trainingErrors;


void startAlgorithm();
int showGraphics();
void processActivationNeuronsLayers(vector<Neuron*>*, vector<float>, vector<float>&);
void processWeightTraining(vector<Neuron*>*, vector<float>, vector<float>, int);
void givePrediction(vector<Neuron*>*, vector<float>, vector<float>&, int);
int interpretOutput(float);

int main() {
	fileName = "Data/SpotifyData.csv";
	desiredOutputs = new vector<vector<float>>();
	inputs = new vector<vector<float>>(); 
	FileManager fileManager(fileName);
	fileManager.readFile(desiredOutputs, inputs);
	startAlgorithm();
	return 0;
}

void startAlgorithm()
{
	///Test data

	//Test weights
	vector<float> weightsNeuron5 = { 0.3503, 0.896, 0.8229, 0.7467, 0.1742 };
	vector<float> weightsNeuron6 = { 0.7106,0.5136, 0.304, 0.015, 0.0915 };
	vector<float> weightsNeuron7 = { 0.1474, 0.1659, 0.9886, 0.4457, 0.1191 };
	vector<float> weightsNeuron8 = { 0.009, 0.3779, 0.5317, 0.5712, 0.6018 };
	vector<float> weightsNeuron9 = { 0.1663, 0.6631, 0.4508, 0.3522, 0.0571 };
	vector<float> weightsNeuron10 = { 0.7834, 0.8027, 0.5199, 0.302, 0.876 };
	vector<float> weightsNeuron11 = { 0.956, 0.9258, 0.5394, 0.1424, 0.4621 };
	vector<float> weightsNeuron12 = { 0.8623, 0.2097, 0.7797, 0.8437, 0.9968 };
	vector<float> weightsNeuron13 = { 0.6115, 0.3925, 0.2663, 0.2973, 0.8402 };
	vector<float> weightsNeuron14 = { 0.3759, 0.0927, 0.6773, 0.0563, 0.0088 };
	vector<float> weightsNeuron15 = { 0.2759, 0.2729, 0.588, 0.6912, 0.8377 };
	vector<float> weightsNeuron16 = { 0.338228, 0.0909095, 0.613415, 0.321976, 0.340997, 0.798585, 0.595067, -0.0587729, 0.493206, 0.237212, 0.631739 };
	vector<float> weightsNeuron17 = { 0.117111, 0.00605023, -0.25479, -0.22722, 0.0628525, 0.337687, 0.0542272, 0.472261, 0.425216, -0.115076, -0.150275 };
	vector<float> weightsNeuron18 = { -0.00318725, -0.321392, 0.257798, 0.0118517, -0.246423, 0.443678, -0.360832, 0.342422, 0.337718, -0.196953, 0.0782752 };
	vector<float> weightsNeuron19 = { 0.474943, 0.300624, -0.285745, 0.387324, 0.430263, 0.0796271, -0.30764, -0.381188, 0.464453, -0.0217566, -0.190207 };
	vector<float> weightsNeuron20 = { 0.291593, 0.0632631, -0.114062, 0.130369, 0.331831, 0.306481, 0.275456, -0.0249823, -0.192197, -0.077262, 0.525198 };
	vector<float> biasLayer1 = { 0.4799, 0.859, 0.3645, 0.0047, 0.6072, 0.6077, 0.7267, 0.2354, 0.9997, 0.0238, 0.9188 };
	vector<float> biasLayer2 = { 0.965424, 1.34722, 1.45938, 0.868405, 1.21753};
	//desiredOutputs = new vector<vector<float>>({ {1}, {0}, {0}, {0} });
	//inputs = new vector<vector<float>>({ { 1, 1 }, {0 , 1}, {1, 0}, {0, 0} });
	//NeuronLayer* nl1 = new NeuronLayer(2, "inputLayer", 0);
	//NeuronLayer* nl2 = new NeuronLayer(2, "hiddenLayer", 2);
	//NeuronLayer* nl3 = new NeuronLayer(1, "outputLayer", 2);
	//Random weights
	//Spotify Dataset
	//NeuronLayer* nl1 = new NeuronLayer(5, "inputLayer", 0);
	//NeuronLayer* nl2 = new NeuronLayer(11, "hiddenLayer", 5);
	//NeuronLayer* nl3 = new NeuronLayer(5, "outputLayer", 11);
	vector<vector<float>> weightsHiddenLayer = { weightsNeuron5,weightsNeuron6,weightsNeuron7,weightsNeuron8,weightsNeuron9,
	weightsNeuron10, weightsNeuron11, weightsNeuron12, weightsNeuron13, weightsNeuron14, weightsNeuron15 };
	vector<vector<float>> weightsOutputLayer = { weightsNeuron16, weightsNeuron17, weightsNeuron18, weightsNeuron19, weightsNeuron20 };
	NeuronLayer* nl1 = new NeuronLayer(5, "inputLayer", 0);
	NeuronLayer* nl2 = new NeuronLayer(11, "hiddenLayer", 5, weightsHiddenLayer, biasLayer1);
	NeuronLayer* nl3 = new NeuronLayer(5, "outputLayer", 11, weightsOutputLayer, biasLayer2);

	//Iris Dataset
	//NeuronLayer* nl1 = new NeuronLayer(4, "inputLayer", 0);
	//NeuronLayer* nl2 = new NeuronLayer(9, "hiddenLayer", 4);
	//NeuronLayer* nl3 = new NeuronLayer(3, "outputLayer", 9);


	
	nl1->connectNeurons(nl2);
	nl2->connectNeurons(nl3);


	// cout << "------------------------------------------------------------" << endl; 
	// cout << "INITIAL WEIGHTS:" << endl;
	// cout << "------------------------------------------------------------" << endl;
	// cout << nl1->toString() << endl;
	// cout << nl2->toString() << endl;
	// cout << nl3->toString() << endl;

	//Start iterations
	int count = 0;
	int epocs = 0;
	int size = inputs->size();
	float lastLearningRate = 0.0;

	std::chrono::time_point<std::chrono::system_clock> instanteInicial, instanteFinal;
	instanteInicial = std::chrono::system_clock::now();

	for (int indexData = 0; indexData < size; indexData++) {
		vector<Neuron*>* neuronLayer = nl1;
		vector<vector<float>> inputsNextLayer;
		vector<float> input;
		string layerRole = "inputLayer";
		while (!neuronLayer->empty()) {
			Neuron* neuron = neuronLayer->front();
			vector<float> output; //Output is going to be the input of the next layer
			layerRole = neuron->getRole();
			if (layerRole == "inputLayer") {
				inputsNextLayer.push_back(inputs->at(indexData));
				neuronLayer = neuron->getNextLayer();
			} else {
				input = inputsNextLayer.back();
				inputsNextLayer.erase(inputsNextLayer.begin() + inputsNextLayer.size() - 1);
				givePrediction(neuronLayer, input, output, indexData);
				inputsNextLayer.push_back(output);
				neuronLayer = neuron->getNextLayer();
			}
		}
	}

	//while (hits <= size/2){
	//	hits = 0;
	//	/*sumOfSquaredErrors = 0.0;*/
	//	//process layers 
	//	for (int indexData = 0; indexData < size; indexData++) {
	//		vector<Neuron*>* neuronLayer = nl1;
	//		vector<vector<float>> inputsNextLayer;
	//		vector<float> input;
	//		string layerRole;
	//		bool backpropagation = false;

	//		while (neuronLayer->front()->getRole() != "inputLayer" || !backpropagation) {
	//			Neuron* neuron = neuronLayer->front();
	//			vector<float> output; //Output is going to be the input of the next layer
	//			layerRole = neuron->getRole();
	//			if (layerRole == "inputLayer" && !backpropagation) {
	//				inputsNextLayer.push_back(inputs->at(indexData));
	//				neuronLayer = neuron->getNextLayer();
	//			} else {
	//				input = inputsNextLayer.back();
	//				if (!backpropagation) {
	//					processActivationNeuronsLayers(neuronLayer, input, output);
	//					if (neuron->getRole() == "outputLayer") {
	//						backpropagation = true;
	//					}
	//					else {
	//						inputsNextLayer.push_back(output);
	//						neuronLayer = neuron->getNextLayer();
	//					}
	//				} else {
	//					output = outputs.back();
	//					processWeightTraining(neuronLayer, input, output, indexData);
	//					neuronLayer = neuron->getPreviousLayer();
	//					inputsNextLayer.erase(inputsNextLayer.begin() + inputsNextLayer.size() - 1); //Delete back
	//					outputs.erase(outputs.begin() + outputs.size() - 1); //Delete back
	//				}
	//			}
	//		}
	//	}
	///*	cout << "Last SSE: " << lastSSR << endl;
	//	if (lastSSR == sumOfSquaredErrors){
	//		SSRCount++;
	//		if (SSRCount >= 10) {
	//			learningRate += 0.0001;
	//			SSRCount = 0;
	//		}
	//	}
	//	lastSSR = sumOfSquaredErrors;
	//	sumOfSquaredErrors /= size;*/
	//  trainingErrors.push_back(sumOfSquaredErrors);
	//	cout << "Aciertos " << hits << endl;
	//	cout << "Iteration: " << count << endl;
	//	cout << "LR: " << learningRate << endl;
	//	count++;
	//}

	instanteFinal = std::chrono::system_clock::now();
	std::chrono::duration<double> segundos = instanteFinal - instanteInicial;
	cout << "SECONDS: " << segundos.count() << endl;

	cout << "------------------------------------------------------------" << endl;
	cout << "FINAL WEIGHTS:" << endl;
	cout << "------------------------------------------------------------" << endl;
	cout << nl1->toString() << endl;
	cout << nl2->toString() << endl;
	cout << nl3->toString() << endl;
	cout << "------------------------------------------------------------" << endl;
	//showGraphics();
}

void givePrediction(vector<Neuron*>* neuronLayer, vector<float> input, vector<float>& output, int desiredOutputIndex) {
	int size = neuronLayer->size();
	float error = 0.0;
	stringstream desiredOutput;
	stringstream actualOutput;
	for (int neuronIndex = 0; neuronIndex < size; neuronIndex++){
		Neuron* neuron = neuronLayer->at(neuronIndex);
		float value = neuron->activationFunction(input);
		output.push_back(value);
		if (neuron->getRole() == "outputLayer"){
			int output = interpretOutput(value);
			desiredOutput << desiredOutputs->at(desiredOutputIndex)[neuronIndex] << " ";
			actualOutput << output << " ";
			error += neuron->getError(desiredOutputs->at(desiredOutputIndex)[neuronIndex], output);
		}
	}
	cout << "Desired output: " << desiredOutput.str() << endl;
	cout << "Predicted output: " << actualOutput.str() << endl;
	cout << "Error: " << error << endl;
}

void processActivationNeuronsLayers(vector<Neuron*>* neuronLayer, vector<float> input, vector<float>& output) {
	vector<float> outputValues;
	int size = neuronLayer->size();
	for (int neuronIndex = 0; neuronIndex < size; neuronIndex++){
		Neuron* neuron = neuronLayer->at(neuronIndex);
		float value = neuron->activationFunction(input);
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
	bool check = true;
	for (int neuronIndex = 0; neuronIndex < size; neuronIndex++) {
		Neuron* neuron = neuronLayer->at(neuronIndex);
		weights.push_back(neuron->getWeights());
		float errorGradient = 0.0;

		if (role == "outputLayer") { 
			errorGradient = neuron->calculateErrorForOutputLayer(desiredOutputs->at(desiredOutputIndex)[neuronIndex], output[neuronIndex]);
			float error = neuron->getError(desiredOutputs->at(desiredOutputIndex)[neuronIndex], interpretOutput(output[neuronIndex]));
			/*sumOfSquaredErrors += pow(error, 2);*/
			if (error != 0){
				check = false;
			}
		} else {
			errorGradient = neuron->calculateErrorForHiddenLayer(output[neuronIndex], previousLayerErrors, previousWeights, neuronIndex);
		}

		vector<float> newWeights = neuron->updateWeight(learningRate, errorGradient, input);
		neuron->setWeights(newWeights);
		errors.push_back(errorGradient);

	}
	if (check && role == "outputLayer") {
		hits++;
	}
	previousLayerErrors = errors;
	previousWeights = weights;
}

int interpretOutput(float outputValue) {
	if (outputValue < threshold) 
		return 0;
	else 
		return 1;
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