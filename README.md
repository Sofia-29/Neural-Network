# Simple Multilayer Neural Network

## System description

Simple implementation in C++ of a multilayer Neural network based on the algorithm described in the book Artificial Intelligence: A Guide to Intelligent Systems by Michael Negnevitsky. 

## Algorithm by Michael Negnevitsky

### Step 1: Initialization

Set all the weights and threshold levels of the network to random numbers uniformly distributed inside a small range. The weights initialization is done on a neuron-by-neuron basis. 

### Step 2: Activation

Activate the back-propagation neural network by applaying inputs $x_{1}$, $x_{2}$, $x_{3}$, ..., $x_{n}$ and desired outputs $y_{1}$, $y_{2}$, $y_{3}$, ..., $y_{n}$. 
1. Calculate the actual outputs of the neurons in the hidden layer. This process is done by calculating the sigmoid function to the summation of each input times the weight less the bias of a neuron. 
2. Calculate the actual outputs of the neurons in the output layer. This process is done by calculating the sigmoid function to the summation of each input times the weight less the bias of a neuron.

### Step 3: Weight training

Update the weights in the back-propagation network propagating backward the errors associated with output neurons.

1. Calculate the error gradient for the neurons in the output layer. Calculate the weights corrections and then updated the weights at the output neurons.
2. Calculate the error gradient for the neurons in the hidden layer. Calculate the weights corrections and then updated the weights at the hidden neurons.

### Step 4: Iteration

Increased iteration by one, go back to step 2, and repeat the process until the selected error criterion is satisfied.

## How to use

1. Start by opening a WSL terminal in the folder "src/controller/neural_network", then run the FastApi server with the following command: uvicorn neural_network:app --reload
2. Open a terminal in Powershell go to src/view and run the following command: yarn
3. In the same Poweshell terminal, run the following command: yarn dev
Then you can check out the page in http://127.0.0.1:5173/
