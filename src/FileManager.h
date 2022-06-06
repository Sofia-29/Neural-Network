#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>

using namespace std;

class FileManager
{
public:
	FileManager(string);
	~FileManager();

	void readFile(vector<vector<float>>* desiredOutputs, vector<vector<float>>* inputData);
    string printResults();

private:
	string fileName;
	vector < pair<string, vector<string>>> results;
};


FileManager::FileManager(string file) 
{
    fileName = file;
}

FileManager::~FileManager()
{
}


//Adapted from: https://www.gormanalysis.com/blog/reading-and-writing-csv-files-with-cpp/
void FileManager::readFile(vector<vector<float>>* desiredOutputs, vector<vector<float>>* inputData)
{
	ifstream myFile(fileName);

	if (!myFile.is_open()) throw runtime_error("Could not open file");

	string line, columnname;
	string value;

	while (getline(myFile, line)) {
		vector<float> desiredOutput;
		vector<float> input;
		stringstream ss(line);
		int counter = 0;
		string token = "";
		while (getline(ss, token, ',')) {
				float value = stof(token);
				if (counter >= 5 && counter <= 9) {
					desiredOutput.push_back(value);
				}
				else {
					input.push_back(value);
				}
			if (ss.peek() == ',') ss.ignore();
			counter++;	
		}
		desiredOutputs->push_back(desiredOutput);
		inputData->push_back(input);
	}
	myFile.close();
}


string FileManager::printResults()
{
    ostringstream salida;
    salida << "Archivo csv:" << endl;
    for (int index1 = 0; index1 < results.size(); index1++){
        salida << results[index1].first;
            for (int index2 = 0; index2 < results[index1].second.size(); index2++)
            {
                salida << results[index1].second[index2] << endl;
            }
    }
        
    return salida.str();

}

