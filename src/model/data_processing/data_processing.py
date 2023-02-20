import ctypes
import pandas as pd

neural_network_caller = ctypes.CDLL('./neuralNetworkCaller.so')
threshold = 0.5
learning_rate = 0.5;

def read_csv_file(name : str) -> pd.DataFrame:
    dataframe = pd.read_csv(name, header=None, sep=',')
    return dataframe

def fill_pointer_list(ponter_list: ctypes.POINTER, list: list,
                      rows_count: int, columns_count: int) -> None:
    for i in range(rows_count):
        ponter_list[i] = (ctypes.c_float * columns_count)(*list[i])
    return


def train_dataset(x_train_dataset: pd.DataFrame, y_train_dataset: pd.DataFrame) -> None:

    x_rows_count = x_train_dataset.shape[0]
    x_column_count = x_train_dataset.shape[1]
    input_pointer = (ctypes.POINTER(ctypes.c_float) * x_rows_count)()
    input_list = x_train_dataset.values.tolist()
    fill_pointer_list(input_pointer, input_list, x_rows_count, x_column_count)

    y_rows_count = y_train_dataset.shape[0]
    y_column_count = y_train_dataset.shape[1]
    desired_output_pointer = (ctypes.POINTER(ctypes.c_float) * y_rows_count)()
    desired_output_list = y_train_dataset.values.tolist()
    fill_pointer_list(desired_output_pointer,
                      desired_output_list, y_rows_count, y_column_count)

    number_of_input_neurons = x_column_count
    number_of_output_neurons = y_column_count

    global neural_network_caller
    run_data_training = neural_network_caller.runDataTraining
    run_data_training.argtypes = [ctypes.c_float, ctypes.c_float, ctypes.c_int, ctypes.c_int, ctypes.POINTER(
        ctypes.c_float) * x_rows_count, ctypes.POINTER(ctypes.c_float) * y_rows_count, ctypes.c_int]
    run_data_training.restype = None

    global threshold, learning_rate
    run_data_training(threshold, learning_rate, number_of_input_neurons,
                    number_of_output_neurons, input_pointer, desired_output_pointer, x_rows_count)

    return


def main():
    # inputs = read_csv_file('SpotifyTrainingInputs.csv')
    # desired_outputs = read_csv_file('SpotifyTrainingDesiredOutputs.csv')

    inputs = read_csv_file('XOR_operation_inputs.csv')
    desired_outputs = read_csv_file('XOR_operation_desired_outputs.csv')

    train_dataset(inputs, desired_outputs)
    return

main()