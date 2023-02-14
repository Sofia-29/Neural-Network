#include <iostream>
#include <vector>

#include "Algorithm.h"

extern "C" void runDataTraining(float, float, int, int,
                                float**, float**, int);

void runDataTraining(float threshold, float learningRate, int numberOfInputsNeurons, int numberOfOutputNeurons,
                                float** inputs, float** desiredOutputs, int size)
{

    // float threshold = 0.5;
    // float learningRate = 0.1 
    Algorithm algorithm(threshold, learningRate);

    vector<vector<float>> vec;
    for (int i = 0; i < size; i++)
    {   
        vector<float> vecAux(numberOfInputsNeurons);
        for (int j = 0; j < numberOfInputsNeurons; j++)
        {
            vecAux[j] = inputs[i][j];
        }
        vec.push_back(vecAux);
    }

    vector<vector<float>> vecDO;
    for (int i = 0; i < size; i++)
    {   
        vector<float> vecAux(numberOfOutputNeurons);
        for (int j = 0; j < 1; j++)
        {
            vecAux[j] = desiredOutputs[i][j];
        }
        vecDO.push_back(vecAux);
    }

    algorithm.startAlgorithm(numberOfInputsNeurons, numberOfOutputNeurons, vec, vecDO);
}
