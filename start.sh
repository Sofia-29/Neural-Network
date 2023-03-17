cd src
cd model

archivo="shared_object/neuralNetworkCaller"

if test -f "$archivo"; then
  rm "$archivo" 
fi

g++ -shared -o shared_object/neuralNetworkCaller.so -fPIC neural_network/NeuralNetworkCaller.cpp
