cd src
cd model
g++ -shared -o data_processing/neuralNetworkCaller.so -fPIC neural_network/NeuralNetworkCaller.cpp
cd data_processing
ls
python3 data_processing.py