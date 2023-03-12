#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class FileManager
{
public:
	FileManager();
	~FileManager();

	void saveCSV(const vector<float>);
};


FileManager::FileManager() {}

FileManager::~FileManager() {}

void FileManager::saveCSV(const vector<float> trainingErrors)
{
	string output_path = "../model/training_errors/training_errors.csv";
    ofstream outfile(output_path);

    if (!outfile.is_open()) {
        cerr << "Error: Could not open file \"" << output_path << "\" for writing." << endl;
        return;
    }

    for (const auto& val : trainingErrors) {
        outfile << val << ",";
    }
    outfile << endl;

    outfile.close();
}

