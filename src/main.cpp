#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <Python.h>
#include <numpy/arrayobject.h>
#include "Neuron.h"
#include "NeuronLayer.h"
#include "FileManager.h"

using namespace std;

float threshold = 0.5;
float learningRate = 0.001;
int numberOfInputNeurons = 0;
int numberOfOutputNeurons = 0;
float sumOfSquaredErrors = 1;
vector<vector<float>>* inputs;
vector<vector<float>> outputs;
vector<vector<float>> previousWeights;
vector<vector<float>>* desiredOutputs;
vector<vector<float>>* weights;
vector<float> previousLayerErrors;
vector<float> trainingErrors; 
FileManager fileManager{};

int hits = 0;
int errorCounter = 0;

void startAlgorithm();
void processActivationNeuronsLayers(vector<Neuron*>*, vector<float>, vector<float>&);
void givePrediction(vector<Neuron*>*, vector<float>, vector<float>&, int);
void processWeightTraining(vector<Neuron*>*, vector<float>, vector<float>, int);
int interpretOutput(float);
int showGraphics();
void loadWeights(string, NeuronLayer*);

int main() {

	startAlgorithm();
	
	return 0; 
}

void startAlgorithm()
{

	//Spotify Dataset
	int opcion = 0;
	while (opcion != 3) {
		cout << "\n1. Entrenar red con un conjunto de datos \n";
		cout << "2. Dar predicciones \n";
		cout << "3. Salir \n";
		cout << "Digite el numero de la opcion que desea escoger: ";
		cin >> opcion;
		if (opcion == 1) {
			desiredOutputs = new vector<vector<float>>();
			inputs = new vector<vector<float>>();
			weights = new vector<vector<float>>();

			string fileInputName = "";
			string fileDesiredOutputName = "";
			cout << "\nDigite el nombre del archivo con los datos a clasificar: ";
			cin >> fileInputName;
			cout << "\nDigite el nombre del archivo a donde se encuentran las salidas esperadas: ";
			cin >> fileDesiredOutputName;
			cout << "\nDigite la cantidad de neuronas de la capa de entrada: ";
			cin >> numberOfInputNeurons;
			cout << "\nDigite la cantidad de neuronas de la capa de salida: ";
			cin >> numberOfOutputNeurons;

			int numberOfHiddenNeurons = numberOfInputNeurons * 2 + 1;
			NeuronLayer* nl1 = new NeuronLayer(numberOfInputNeurons, "inputLayer", 0);
			NeuronLayer* nl2 = new NeuronLayer(numberOfHiddenNeurons, "hiddenLayer", numberOfInputNeurons);
			NeuronLayer* nl3 = new NeuronLayer(numberOfOutputNeurons, "outputLayer", numberOfHiddenNeurons);
			fileManager.readFile(inputs, "Data/" + fileInputName + ".csv");
			fileManager.readFile(desiredOutputs,  "Data/" + fileDesiredOutputName + ".csv");
			
			nl1->connectNeurons(nl2);
			nl2->connectNeurons(nl3);
			int count = 0;
			int size = inputs->size();

			std::chrono::time_point<std::chrono::system_clock> instanteInicial, instanteFinal;
			instanteInicial = std::chrono::system_clock::now();

			while (hits <= size/2){
				hits = 0;
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
							inputsNextLayer.push_back(inputs->at(indexData));
							neuronLayer = neuron->getNextLayer();
						} else {
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
				sumOfSquaredErrors /= size;
				trainingErrors.push_back(sumOfSquaredErrors);
				cout << "Aciertos " << hits << endl;
				cout << "Iteration: " << count << endl;
				count++;
			}

			instanteFinal = std::chrono::system_clock::now();
			std::chrono::duration<double> segundos = instanteFinal - instanteInicial;
			cout << "SECONDS: " << segundos.count() << endl;
			showGraphics();
			fileManager.createFile(nl1,  "Data/" + fileInputName);
			delete nl1;
			delete nl2;
			delete nl3;
			delete desiredOutputs;
			delete inputs;
			delete weights;
		} else if (opcion == 2) {
			desiredOutputs = new vector<vector<float>>();
			inputs = new vector<vector<float>>();
			weights = new vector<vector<float>>();
			errorCounter = 0;
			string fileTrainingName = "";
			string  fileInputName = "";
			string  fileDesiredOutputName = "";
			cout << "\nDigite el nombre del archivo donde se encuentran los datos de entrenamiento: ";
			cin >> fileTrainingName;
			cout << "\nDigite el nombre del archivo que desea clasificar: ";
			cin >> fileInputName;
			cout << "\nDigite el nombre del archivo a donde se encuentran las salidas esperadas: ";
			cin >> fileDesiredOutputName;
			cout << "\nDigite la cantidad de neuronas de la capa de entrada: ";
			cin >> numberOfInputNeurons;
			cout << "\nDigite la cantidad de neuronas de la capa de salida: ";
			cin >> numberOfOutputNeurons;

			fileManager.readFile(inputs,  "Data/" + fileInputName + ".csv");
			fileManager.readFile(desiredOutputs,  "Data/" + fileDesiredOutputName + ".csv");
			int numberOfHiddenNeurons = numberOfInputNeurons * 2 + 1;
			NeuronLayer* nl1 = new NeuronLayer(numberOfInputNeurons, "inputLayer", 0);
			NeuronLayer* nl2 = new NeuronLayer(numberOfHiddenNeurons, "hiddenLayer", numberOfInputNeurons);
			NeuronLayer* nl3 = new NeuronLayer(numberOfOutputNeurons, "outputLayer", numberOfHiddenNeurons);
			nl1->connectNeurons(nl2);
			nl2->connectNeurons(nl3);


			loadWeights( "Data/" + fileTrainingName + ".csv", nl1);
			//Start iterations
			int count = 0;
			int size = inputs->size();

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
					}
					else {
						input = inputsNextLayer.back();
						inputsNextLayer.erase(inputsNextLayer.begin() + inputsNextLayer.size() - 1);
						givePrediction(neuronLayer, input, output, indexData);
						inputsNextLayer.push_back(output);
						neuronLayer = neuron->getNextLayer();
					}
				}
			}

			cout << "\nCantidad de predicciones erroneas: " << errorCounter << " de " << size << " datos. \n";
			delete nl1;
			delete nl2;
			delete nl3;
			delete desiredOutputs;
			delete inputs;
			delete weights;
		} else {
			break;
		}
	}
}

void givePrediction(vector<Neuron*>* neuronLayer, vector<float> input, vector<float>& output, int desiredOutputIndex) {
	int size = neuronLayer->size();
	float error = 0.0;
	stringstream desiredOutput;
	stringstream actualOutput;
	for (int neuronIndex = 0; neuronIndex < size; neuronIndex++) {
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
	if(error != 0)
		errorCounter++;
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
			sumOfSquaredErrors += pow(error, 2);
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

void loadWeights(string fileName, NeuronLayer* inputLayer)
{
	vector<Neuron*>* neuronLayer = inputLayer->at(0)->getNextLayer();
	weights = new vector<vector<float>>();
	fileManager.readFile(weights, fileName);
	int weightIndex = 0;
	while(!neuronLayer->empty()){
		for (int index = 0; index < neuronLayer->size(); index++) {
			Neuron* neuron = neuronLayer->at(index);
			vector<float> weight = weights->at(weightIndex);
			float bias = weight[weight.size() - 1];
			weight.erase(weight.begin() + weight.size() - 1);
			neuron->setWeights(weight);
			neuron->setBias(bias);
			weightIndex++;
		}
		neuronLayer = neuronLayer->at(0)->getNextLayer();
	}
}

int showGraphics() {
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