#include <iostream>
#include <vector>

#include "Algorithm.h"

using namespace std;

extern "C" void runDataTraining(float, float, int, int,
                                float**, float**, int);

extern "C" void tryModel(float, float, int, int,
                                float**, float**, int, const char*);

void convertToSTLVector(vector<vector<float>>&, float**, int);
void printVector(vector<vector<float>>);

void runDataTraining(float threshold, float learningRate, int numberOfInputsNeurons, int numberOfOutputNeurons,
                     float **inputsPointer, float **desiredOutputsPointer, int size)
{
    Algorithm algorithm(threshold, learningRate);

    vector<vector<float>> inputs(size);
    convertToSTLVector(inputs, inputsPointer, numberOfInputsNeurons);

    vector<vector<float>> desiredOutputs(size);
    convertToSTLVector(desiredOutputs, desiredOutputsPointer, numberOfOutputNeurons);

    algorithm.startTraining(numberOfInputsNeurons, numberOfOutputNeurons, inputs, desiredOutputs);
}


void tryModel(float threshold, float learningRate, int numberOfInputsNeurons, int numberOfOutputNeurons,
                     float **inputsPointer, float **desiredOutputsPointer, int size, const char* fileName)
{
    string file(fileName);
    Algorithm algorithm(threshold, learningRate);
    
    vector<vector<float>> inputs(size);
    convertToSTLVector(inputs, inputsPointer, numberOfInputsNeurons);

    vector<vector<float>> desiredOutputs(size);
    convertToSTLVector(desiredOutputs, desiredOutputsPointer, numberOfOutputNeurons);

    algorithm.testNeuralNetwork(fileName,numberOfInputsNeurons, numberOfOutputNeurons, inputs, desiredOutputs);
}

void convertToSTLVector(vector<vector<float>> &stlVector, float **pointerVector, int size)
{
    for(int rowIndex = 0; rowIndex < stlVector.size(); rowIndex++){
        vector<float> rowVector(size); 
        for(int columnIndex = 0; columnIndex < size; columnIndex++){
            rowVector[columnIndex] = pointerVector[rowIndex][columnIndex];
        }
        stlVector[rowIndex] = rowVector;
    }
}

void printVector(vector<vector<float>> vector){

    for(int rowIndex = 0; rowIndex < vector.size(); rowIndex++){
        for(int columnIndex = 0; columnIndex < vector[rowIndex].size(); columnIndex++){
            cout << " " << vector[rowIndex][columnIndex];
        }
        cout << endl; 
    }

}
