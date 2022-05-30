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

	void readFile();
    void readAndFilterData(vector<vector<float>>&, vector<float>);
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
void FileManager::readFile() 
{
	string line, columnName, value;
	ifstream file(fileName);

    if (!file.is_open()) throw runtime_error("Could not open file");

    if (file.good())
    {
        getline(file, line);

        stringstream ss(line);

        while (getline(ss, columnName, ',')) {
            if (columnName == "top genre") {
                results.push_back({ columnName, std::vector<string> {} });
            }
        }

        while (std::getline(file, line))
        {
            int colIdx = 0;
            stringstream ss(line);

            while (getline(ss, value, ',')) {   
                if (colIdx == 2) {
                    results[0].second.push_back((value));
                    break;
                }
                if (ss.peek() == ',') ss.ignore();
                colIdx++;
            }
        }

        file.close();
    }
}

inline void FileManager::readAndFilterData(vector<vector<float>>&, vector<float>)
{
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

