#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include "Neuron.h"

using namespace std;

class FileManager
{
public:
	FileManager();
	~FileManager();

	void createFile(vector<Neuron*>*, string);

};


FileManager::FileManager() 
{
}

FileManager::~FileManager()
{
}

void FileManager::createFile(vector<Neuron*>* neuronLayer, string fileName)
{
	fstream fout;
	fout.open(fileName + "Weights.csv", ios::out | ios::app);
	vector<Neuron*>* neuronLayerAuxiliar = neuronLayer->at(0)->getNextLayer();
	while (!neuronLayerAuxiliar->empty()) {
		
		for (int index = 0; index < neuronLayerAuxiliar->size(); index++) {
			Neuron* neuron = neuronLayerAuxiliar->at(index);
			stringstream ss(neuron->toString());
			string weight = "";
			while (getline(ss, weight, ',')) {
				fout << weight << ", ";
			}
			fout << "\n";
		}
		neuronLayerAuxiliar = neuronLayerAuxiliar->at(0)->getNextLayer();
	}
}


