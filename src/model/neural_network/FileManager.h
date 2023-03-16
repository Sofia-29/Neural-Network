#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Neuron.h"

using namespace std;

class FileManager
{
public:
	FileManager();
	~FileManager();

    void createWeightsFile(vector<Neuron*>*);
    void readResultsWeightsCSV(vector<vector<float>>*);
	void saveCSV(const vector<float>);
    void saveResults(const vector<vector<float>>, string);
};


FileManager::FileManager() {}

FileManager::~FileManager() {}

void FileManager::saveResults(const vector<vector<float>> results, string name)
{
    string output_path = name;
    ofstream outfile(output_path);
    if(!outfile.is_open()) {
        cerr << "Error: Could not open file \"" << output_path << "\" for writing." << endl;
        return;
    }

    for (const auto& row : results) {
        for (const auto& val: row){
            outfile << val << ",";
        }
        outfile << "\n";
    }
}

void FileManager::saveCSV(const vector<float> trainingErrors)
{
	string output_path = "../results_files/training_errors/training_errors.csv";
    ofstream outfile(output_path);

    if(!outfile.is_open()) {
        cerr << "Error: Could not open file \"" << output_path << "\" for writing." << endl;
        return;
    }

    for (const auto& val : trainingErrors) {
        outfile << val << ",";
    }
    outfile << endl;

    outfile.close();
}

void FileManager::createWeightsFile(vector<Neuron*>* neuronLayer){
    string output_path = "../results_files/weights/weights.csv";
    ofstream outfile(output_path);

    if (!outfile.is_open()) {
        cerr << "Error: Could not open file \"" << output_path << "\" for writing." << endl;
        return;
    }

    vector<Neuron*>* neuronLayerAuxiliar = neuronLayer->at(0)->getNextLayer();
	while (!neuronLayerAuxiliar->empty()) {

		for (int index = 0; index < neuronLayerAuxiliar->size(); index++) {
			Neuron* neuron = neuronLayerAuxiliar->at(index);
			stringstream ss(neuron->toString());
			string weight = "";
			while (getline(ss, weight, ',')) {
				outfile << weight << ", ";
			}
			outfile << "\n";
		}
		neuronLayerAuxiliar = neuronLayerAuxiliar->at(0)->getNextLayer();
	}
}

void FileManager::readResultsWeightsCSV(vector<vector<float>>* data){
    ifstream myFile("../results_files/weights/weights.csv");

	if (!myFile.is_open()) throw runtime_error("Could not open file");

    string line = "";


    while (getline(myFile, line)) {
        vector<float> input;
		stringstream ss(line);
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