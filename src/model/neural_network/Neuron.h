#pragma once
#include <vector>
#include <sstream>
#include <string>
#include <random>
#include <Functions.h>

using namespace std;

class Neuron
{
public:
	Neuron(string, int);
	~Neuron();

	void initialization(int);
	string getRole();
	vector<float> getWeights();
	vector<Neuron*>* getNextLayer();
	vector<Neuron*>* getPreviousLayer();

	void setWeights(vector<float>);
	void setBias(float);

	void addIncomingNeuron(Neuron*);
	void addOutgoingNeuron(Neuron*);
	float activationFunction(vector<float>);
	vector<float> updateWeight(float, float, vector<float>);

	// float sigmoidFunction(float);
	// float sigmoidDerivatedFunction(float);
	// float getError(float, float);
	// float calculateErrorForOutputLayer(float, float);
	// float calculateErrorForHiddenLayer(float, vector<float>, vector<vector<float>>, int);

	Neuron* begin() { return this; };
	Neuron* end() { return this + 1; };
	string toString();

private:
	vector<Neuron*>* incomingNeurons;
	vector<Neuron*>* outgoingNeurons;

	vector<float> weights;
	string role;
	float bias;

	Functions functions;
};

Neuron::Neuron(string role, int numberOfInputs)
{
	this->role = role;
	this->incomingNeurons = new vector<Neuron*>();
	this->outgoingNeurons = new vector<Neuron*>();
	this->initialization(numberOfInputs);
}

Neuron::~Neuron()
{
	delete incomingNeurons;
	delete outgoingNeurons;
}

void Neuron::initialization(int numberOfInputs)
{
	weights.reserve(numberOfInputs);
	bias = ceil((float)(rand()) / ((float)(RAND_MAX / (1))) * 10000) / 10000;

	//The neuron weights are initialized with a value between 0 and 1. 
	for (int index = 0; index < numberOfInputs; index++)
	{
		float value = (float)(rand()) / ((float)(RAND_MAX / (1)));
		value = ceil(value * 10000) / 10000;
		weights.emplace_back(value);
	}
}

inline string Neuron::getRole()
{
	return this->role;
}

inline vector<float> Neuron::getWeights()
{
	return this->weights;
}

inline vector<Neuron *> *Neuron::getNextLayer()
{
	return outgoingNeurons;
}

inline vector<Neuron *> *Neuron::getPreviousLayer()
{
	return incomingNeurons;
}

inline void Neuron::setWeights(vector<float> newWeights)
{
	this->weights = newWeights;
}

inline void Neuron::setBias(float bias)
{
	this->bias = bias;
}

inline void Neuron::addIncomingNeuron(Neuron *neuron)
{
	incomingNeurons->push_back(neuron);
}

inline void Neuron::addOutgoingNeuron(Neuron *neuron)
{
	outgoingNeurons->push_back(neuron);
}

float Neuron::activationFunction(vector<float> input)
{
	int size = input.size();
	float sum = 0.0;
	for (int index = 0; index < size; index++)
	{
		sum += input[index] * this->weights[index];
	}

	sum -= bias;
	return this->functions.sigmoidFunction(sum);
}

vector<float> Neuron::updateWeight(float learningRate, float errorGradient, vector<float> input)
{
	vector<float> newWeights;
	for (int index1 = 0; index1 < weights.size(); index1++)
	{
		float weightCorrection = learningRate * input[index1] * errorGradient;
		float newWeight = weights[index1] + weightCorrection;
		newWeights.push_back(newWeight);
	}
	this->bias += (learningRate * -1 * errorGradient);
	return newWeights;
}

string Neuron::toString()
{
	ostringstream os;
	for (int index = 0; index < weights.size(); index++)
	{
		os << weights[index] << ",";
	}
	os << this->bias;
	return os.str();
}
