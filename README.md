# Simple Multilayer Neural Network

## System description

Simple system to show the implementation in C++ of a multilayer Neural network based on the algorithm described in the book Artificial Intelligence: A Guide to Intelligent Systems by Michael Negnevitsky. 

## System requirements

- Windows 10 or higher. 
- Windows Subsystem for Linux (WSL).
- At least one internet browser such as Chrome, Firefox or Microsoft Edge.
- FastApi.
- Node.js. 
- You can use npm or yarn as a package manager. To develop and test the project I used yarn.

## Data requirements

- The input to this little system needs to be a relatively small dataset in CSV format.
- In case the dataset has an ID column make sure that it is positioned in the first column or erased it.
- The last column of the dataset must be the expected output or categories to predict.
- Also, the input dataset must not have null values.

## How to use

1. Start by opening a WSL terminal in the folder "src/controller", run the FastApi server with the following command: uvicorn neural_network:app --reload
2.  Then open a terminal in Powershell, go to the folder src/view and run the following command: yarn
3. In the same Poweshell terminal, run the following command: yarn dev
After that proccess is done, you can check out the page in http://127.0.0.1:5173/


## Disclaimer

- I'm using a fixed learning rate value of 0.5, if you want to test another one you can change it in the file DatasetPreprocessing.py in the folder path src/model/dataset_processing.
- The topology chosen for the neural network is same number of input neurons and hidden neurons, the number of neurons in the output layer depends on the number of categories. 
- The error criteria that I used is the sum of squared errors, the value is fixed to be less than 0.01, so if you want to change it you need to go to the folder src/model/neural_network to the file Algorithm.h in code line 110. 
- The examples present in the folder dataset_examples are the ones that I used to test the neural network, to test the system with the XOR operation you need to unmark the following options presented on the home page: Normalize data (because the inputs are binary so it is not necessary), Map out labels ( the output only will be 0 or 1) and split dataset (it only has 4 inputs, so it's not necessary to split it). If you want to test the Iris dataset, I strongly recommend that you keep all the options marked, this guarantee that the neural network performs at best.

## Algorithm by Michael Negnevitsky

You can find the mathematical explanation and examples in section 6.4 of the book Artificial Intelligence: A Guide to Intelligent Systems.

### Step 1: Initialization

Set all the weights and threshold levels of the network to random numbers uniformly distributed inside a small range. The weights initialization is done on a neuron-by-neuron basis. 

### Step 2: Activation

Activate the back-propagation neural network by applying inputs and desired outputs to the net. 

1. Calculate the actual outputs of the neurons in the hidden layer. This process is done by calculating the sigmoid function to the summation of each input times the weight less the bias of a neuron. 
2. Calculate the actual outputs of the neurons in the output layer. This process is done by calculating the sigmoid function to the summation of each input times the weight less the bias of a neuron.

### Step 3: Weight training

Update the weights in the back-propagation network propagating backward the errors associated with output neurons.

1. Calculate the error gradient for the neurons in the output layer. Calculate the weights corrections and then updated the weights at the output neurons.
2. Calculate the error gradient for the neurons in the hidden layer. Calculate the weights corrections and then updated the weights at the hidden neurons.

### Step 4: Iteration

Increased iteration by one, go back to step 2 and repeat the process until the selected error criterion is satisfied.

