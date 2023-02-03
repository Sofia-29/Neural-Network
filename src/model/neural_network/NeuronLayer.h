#pragma once
#include <vector>
#include <sstream>
#include <string>

#include "Neuron.h"

using namespace std;

class NeuronLayer : public vector<Neuron *>
{
public:
	NeuronLayer();
	NeuronLayer(int, string, int);
	~NeuronLayer();
	void connectNeurons(NeuronLayer *);
	string toString();
};

NeuronLayer::NeuronLayer() {}

NeuronLayer::NeuronLayer(int count, string role, int amountOfEntries)
{
	this->reserve(count);
	while (count-- > 0)
	{
		this->push_back(new Neuron(role, amountOfEntries));
	}
}

NeuronLayer::~NeuronLayer()
{
	for (int index = 0; index < this->size(); index++)
	{
		delete this->at(index);
	}
}

inline void NeuronLayer::connectNeurons(NeuronLayer *neuronLayer)
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
