// C++ code
#include <iostream>
#include <vector>

using namespace std;

extern "C" void my_function(float **array, int rows, int cols);

void my_function(float **array1, int rows, int cols)
{
    vector<vector<float>> vec;
    for (int i = 0; i < rows; i++)
    {   
        vector<float> vecAux(cols);
        for (int j = 0; j < cols; j++)
        {
            vecAux[j] = array1[i][j];
        }
        vec.push_back(vecAux);
    }
}