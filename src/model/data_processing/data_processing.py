import ctypes

my_lib = ctypes.CDLL('./neuralNetworkCaller.so')

rows = 4
cols = 2

input_list = [[1.0, 1.0], [0.0, 1.0], [1.0, 0.0], [0.0, 0.0]]

# Create a C-style two-dimensional array and copy the data into it
input = (ctypes.POINTER(ctypes.c_float) * rows)()
for i in range(rows):
    input[i] = (ctypes.c_float * cols)(*input_list[i])


desired_output_list = [[0.0], [1.0], [1.0], [0.0]]

# Create a C-style two-dimensional array and copy the data into it
desired_output = (ctypes.POINTER(ctypes.c_float) * rows)()
for i in range(rows):
    desired_output[i] = (ctypes.c_float * 1)(*desired_output_list[i])

# threshold = 0.5
# learning_rate = 0.1
number_of_input_neurons = 2
number_of_output_neurons = 1

# Call the C++ function with the two-dimensional array
my_lib.runDataTraining(number_of_input_neurons,
                       number_of_output_neurons, input, desired_output, rows)
