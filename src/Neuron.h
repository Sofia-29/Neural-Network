#pragma once
#include <vector>
#include <sstream>
#include <string>
#include <random>
#include <math.h>

using namespace std;

class Neuron 
{
public:
	Neuron(string, int, vector<float>, float);
	Neuron(string, vector<float>, float);
	Neuron(string, int);
	~Neuron();

	void deleteNeurons(vector<Neuron*>*);
	unsigned int gId();
	string getRole();
	vector<Neuron*>* getNextLayer();
	vector<Neuron*>* getPreviousLayer();
	vector<float> getWeights();
	void addOutgoingNeuron(Neuron*);
	void addIncomingNeuron(Neuron*);
	void initialisation(int);
	void setWeights(vector<float>);
	float activationFunction(vector<float>);
	float sigmoidFunction(float);
	float sigmoidDerivatedFunction(float);
	float getError(float, float);
	float calculateErrorForOutputLayer(float, float);
	float calculateErrorForHiddenLayer(float, vector<float>, vector<vector<float>>, int);
	vector<float> updateWeight(float, float, vector<float>);
	Neuron* begin() { return this; };
	Neuron* end() { return this + 1; };
	string toString();

private:
	static unsigned int idg;
	unsigned int id;

	vector<Neuron*>* incomingNeurons;
	vector<Neuron*>* outgoingNeurons;
	// 0     1
//vector<vector<float>> weights;   //w13, w14
								// w23, w24
	vector<float> weights;
	string role;
	float bias;
};

unsigned int Neuron::idg = 0;

Neuron::Neuron(string role, int id, vector<float> weight, float bias)
{
	this->id = Neuron::idg++;
	this->role = role;
	this->weights = weight;
	this->bias = bias;
	this->incomingNeurons = new vector<Neuron*>();
	this->outgoingNeurons = new vector<Neuron*>();
}

Neuron::Neuron(string role, vector<float> weight, float bias)
{
	this->id = Neuron::idg++;
	this->role = role;
	this->weights = weight;
	this->bias = bias;
	this->incomingNeurons = new vector<Neuron*>();
	this->outgoingNeurons = new vector<Neuron*>();
	//	this->incomingNeurons->reserve(amountOfIncomingNeurons);
	//	this->outgoingNeurons->reserve(amountOfOutgoingNeurons);
}

inline Neuron::Neuron(string role, int numberOfInputs)
{
	this->id = Neuron::idg++;
	this->role = role;
	this->incomingNeurons = new vector<Neuron*>();
	this->outgoingNeurons = new vector<Neuron*>();
	this->initialisation(numberOfInputs);
}

Neuron::~Neuron()
{
	deleteNeurons(incomingNeurons);
	deleteNeurons(outgoingNeurons);
}

inline void Neuron::deleteNeurons(vector<Neuron*>* neurons)
{
	neurons->erase(neurons->begin(), neurons->end());
}

unsigned int Neuron::gId()
{
	return id;
}

inline string Neuron::getRole()
{
	return this->role;
}

inline vector<float> Neuron::getWeights()
{
	return this->weights;
}


inline vector<Neuron*>* Neuron::getNextLayer()
{
	return outgoingNeurons;
}

inline vector<Neuron*>* Neuron::getPreviousLayer()
{
	return incomingNeurons;
}


inline void Neuron::addOutgoingNeuron(Neuron* neuron)
{
	outgoingNeurons->push_back(neuron);
}

inline void Neuron::addIncomingNeuron(Neuron* neuron)
{
	incomingNeurons->push_back(neuron);
}

inline void Neuron::initialisation(int numberOfInputs)
{
	weights.reserve(numberOfInputs);
	bias = ceil((float)(rand()) / ((float)(RAND_MAX / (1))) * 10000) / 10000;

	for (int index1 = 0; index1 < numberOfInputs; index1++) {
		float value = (float)(rand()) / ((float)(RAND_MAX / (1)));
		value = ceil(value * 10000) / 10000;
		weights.emplace_back(value);
	}
}

inline void Neuron::setWeights(vector<float> newWeights)
{
	this->weights = newWeights;
}


//<summary>
//Activate the back-propagation neural network by applying inputs
//</summary>
//<param name="inputs">Input values</param>
//<param name="threshold">Threshold value</param>
//<param name="connection">Link between neurons</param>
//<returns> Output of the neuron </returns>
inline float Neuron::activationFunction(vector<float> input)
{
	int size = input.size();
	float sum = 0.0;
	for (int index = 0; index < size; index++) {
		sum += input[index] * this->weights[index];
	}

	sum -= bias;
	return sigmoidFunction(sum);
}

//<summary>
//Sigmoid function
//</summary>
//<param name="sum">Sum of input times weight</param>
//<returns> Result of sigmoid fuction applied to sum </returns>
inline float Neuron::sigmoidFunction(float sum)
{
	return 1 / (1 + exp(-(sum)));
}

//<summary>
//Sigmoid function
//</summary>
//<param name="sum">Sum of input times weight</param>
//<returns> Result of sigmoid fuction applied to sum </returns>
inline float Neuron::sigmoidDerivatedFunction(float output)
{
	return output*(1 - output);
}


inline float Neuron::getError(float desiredOutput, float actualOutput)
{
	return desiredOutput - actualOutput;
}

//<summary>
//Calculates error gradient of the output layer
//</summary>
//<param name="desiredOutput">Desired output</param>
//<param name="actualOutput">Actual output of the net</param>
//<returns> Error gradient of output </returns>
inline float Neuron::calculateErrorForOutputLayer(float desiredOutput, float actualOutput)
{
	float errorGradient = sigmoidDerivatedFunction(actualOutput) * getError(desiredOutput, actualOutput);
	return errorGradient;
}

//<summary>
//Calculates error gradient of the hidden layer
//</summary>
//<param name="actualOutput">Actual output of the net</param>
//<param name="errorGradient">Error gradient of the previous layer</param>
//<param name="oldWeight">Old weight of the neuron</param>
//<returns> Error gradient of neuron in hidden layer </returns>
inline float Neuron::calculateErrorForHiddenLayer(float actualOutput, vector<float> errorGradient, vector<vector<float>> previousWeight, int connection)
{
	float errorGradientHiddenLayer = actualOutput * (1 - actualOutput);
	//float sum = 0.0;
	for (int index = 0; index < errorGradient.size(); index++) {
		errorGradientHiddenLayer *= previousWeight[index][connection] * errorGradient[index];
	}
	//errorGradientHiddenLayer *= sum;
	return errorGradientHiddenLayer;
}


//<summary>
//Update weight of a neuron
//</summary>
//<param name="leanrningRate">Learning rate</param>
//<param name="errorGradient">Error gradient</param>
//<param name="actualVector">Input of a neuron in neurons of hidden layer and output in neurons of output layer</param>
//<param name="oldWeight">Old weight of a neuron</param>
//<returns> New weight of a neuron </returns>
inline vector<float> Neuron::updateWeight(float learningRate, float errorGradient, vector<float> input)
{
	vector<float> newWeights;
	for (int index1 = 0; index1 < weights.size(); index1++) {
		float weightCorrection = learningRate * input[index1] * errorGradient;
		float newWeight = weights[index1] + weightCorrection;
		newWeights.push_back(newWeight);
	}
	this->bias += (learningRate * -1 * errorGradient);
	return newWeights;
}

inline string Neuron::toString()
{
	ostringstream os;
	os << "Id: " << this->id << endl;
	for (int index = 0; index < weights.size(); index++) {
		os << weights[index] << " ";
	}
	os << endl;
	os << "Bias: " << this->bias << endl;
	return os.str();
}

