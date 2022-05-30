#pragma once
#include <vector>
#include <sstream>
#include <string>

#include "Neuron.h"

using namespace std;

class NeuronLayer : public vector < Neuron* >
{
public:
	NeuronLayer(int, string, vector<float>, float, vector<float>, float);
	NeuronLayer(int, string, vector<float>, float);
	NeuronLayer(int, string, int);
	~NeuronLayer();
	void connectNeurons(NeuronLayer*);
	string toString();

};

NeuronLayer::NeuronLayer(int count, string role, vector<float> weights, float bias)
{
	int neuronId = 0;
	this->reserve(count);
	while (count-- > 0)
	{
	/*	this->push_back(new Neuron(role, neuronId, weights, bias));*/
		this->push_back(new Neuron(role, weights, bias));
		neuronId++;
	}
}

inline NeuronLayer::NeuronLayer(int count, string role, int amountOfEntries)
{
	int neuronId = 0;
	this->reserve(count);
	while (count-- > 0)
	{
		this->push_back(new Neuron(role, amountOfEntries));
		neuronId++;
	}
}

NeuronLayer::NeuronLayer(int count, string role, vector<float> weights1, float bias1, vector<float> weights2, float bias2)
{
	this->reserve(count);
	this->push_back(new Neuron(role, weights1, bias1));
	this->push_back(new Neuron(role, weights2, bias2));
}

NeuronLayer::~NeuronLayer()
{
}

inline void NeuronLayer::connectNeurons(NeuronLayer* neuronLayer)
{
	for (int index1 = 0; index1 < this->size(); index1++)
	{
		for (int index2 = 0; index2 < neuronLayer->size(); index2++)
		{
			this->at(index1)->addOutgoingNeuron(neuronLayer->at(index2));
			neuronLayer->at(index2)->addIncomingNeuron(this->at(index1));
		}
	}
}

string NeuronLayer::toString()
{
	ostringstream os;
	os << "Capa de neuronas: " << this->front()->getRole() << endl;
	for (int index = 0; index < this->size(); index++) {
		os << this->at(index)->toString();
	}
	return os.str();
}