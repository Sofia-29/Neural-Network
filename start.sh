cd src
cd model

archivo="neuralNetworkCaller"

if test -f "$archivo"; then
  rm "$archivo" 
fi

g++ -shared -o data_processing/neuralNetworkCaller.so -fPIC neural_network/NeuralNetworkCaller.cpp
cd data_processing
python3 data_processing.py