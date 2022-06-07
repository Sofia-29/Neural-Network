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

	void readFile(vector<vector<float>>*, string);
	void createFile(vector<Neuron*>*, string);

};


FileManager::FileManager() 
{
}

FileManager::~FileManager()
{
}


//Adapted from: https://www.gormanalysis.com/blog/reading-and-writing-csv-files-with-cpp/
void FileManager::readFile(vector<vector<float>>* data, string fileName)
{
	ifstream myFile(fileName);

	if (!myFile.is_open()) throw runtime_error("Could not open file");

	string line = "";

	if (myFile.good())
	{
		//getline(myFile, line);
	}

	while (getline(myFile, line)) {
		vector<float> input;
		stringstream ss(line);
		int counter = 0;
		string token = "";
		while (getline(ss, token, ',')){
			if (token == " ") break;
			float value = stof(token);
			input.push_back(value);
		}
		data->push_back(input);
	}

	myFile.close();

}

inline void FileManager::createFile(vector<Neuron*>* neuronLayer, string fileName)
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


