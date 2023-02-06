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

NeuronLayer::NeuronLayer(int amountOfLayerNeurons, string role, int amountOfPreviousLayerNeurons)
{
	this->reserve(amountOfLayerNeurons);
	while (amountOfLayerNeurons-- > 0)
	{
		this->push_back(new Neuron(role, amountOfPreviousLayerNeurons));
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
	for (int outgoingNeuronIndex = 0; outgoingNeuronIndex < this->size(); outgoingNeuronIndex++)
	{
		for (int incomingNeuronIndex = 0; incomingNeuronIndex < neuronLayer->size(); incomingNeuronIndex++)
		{
			this->at(outgoingNeuronIndex)->addOutgoingNeuron(neuronLayer->at(incomingNeuronIndex));
			neuronLayer->at(incomingNeuronIndex)->addIncomingNeuron(this->at(outgoingNeuronIndex));
		}
	}
}
