import ctypes
import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import MinMaxScaler

from .helpers import fill_pointer_list


class DataProcessing:

    def __init__(self, neural_network_caller: ctypes.CDLL) -> None:
        self.neural_network_caller = neural_network_caller
        self.dataset = None
        self.desired_output = []
        self.desired_output_mapped = {}
        self.splitted_dataset = {}
        self.threshold = 0.5
        self.learning_rate = 0.5

    def start_preprocessing(self, dataset: pd.DataFrame, normalize: bool, 
                            map_desired_output: bool, split_dataset: bool):
        self.dataset = dataset
        self.verify_id_column()
        self.dataset = self.dataset.sample(frac=1)
        if map_desired_output:
            self.map_out_desired_output()
        else:
            last_column = self.dataset.iloc[:, -1].values
            self.desired_output = [[last_column[index]] for index in range(len(last_column))]
        self.dataset.drop(
            columns=self.dataset.columns[-1], axis=1, inplace=True)

        if normalize:
            self.normalize_data()

        if split_dataset:
            self.split_dataset()
        else:
            self.splitted_dataset = {'x_train': self.dataset.values, 'y_train': self.desired_output}
      

    def train_dataset(self) -> None:
        x_train_dataset = self.splitted_dataset['x_train']
        y_train_dataset = self.splitted_dataset['y_train']
        x_rows_count = len(x_train_dataset)
        x_column_count = len(x_train_dataset[0])
        input_pointer = (ctypes.POINTER(ctypes.c_float) * x_rows_count)()
        fill_pointer_list(input_pointer, x_train_dataset,
                          x_rows_count, x_column_count)

        y_rows_count = len(y_train_dataset)
        y_column_count = len(y_train_dataset[0])
        desired_output_pointer = (ctypes.POINTER(
            ctypes.c_float) * y_rows_count)()
        fill_pointer_list(desired_output_pointer,
                          y_train_dataset, y_rows_count, y_column_count)

        number_of_input_neurons = x_column_count
        number_of_output_neurons = y_column_count

        run_data_training = self.neural_network_caller.runDataTraining
        run_data_training.argtypes = [ctypes.c_float, ctypes.c_float, ctypes.c_int, ctypes.c_int, ctypes.POINTER(
            ctypes.c_float) * x_rows_count, ctypes.POINTER(ctypes.c_float) * y_rows_count, ctypes.c_int]
        run_data_training.restype = None
        run_data_training(self.threshold, self.learning_rate, number_of_input_neurons,
                          number_of_output_neurons, input_pointer, desired_output_pointer, x_rows_count)

    def verify_id_column(self) -> None:
        first_row = self.dataset.iloc[0]
        counter = 0
        for column_name in first_row:
            if isinstance(column_name, str) and 'id' == column_name.lower():
                self.dataset.drop(counter, axis=1, inplace=True)
            counter += 1

    def normalize_data(self) -> None:
        scaler = MinMaxScaler()
        self.dataset = pd.DataFrame(scaler.fit_transform(
            self.dataset), columns=self.dataset.columns)

    def split_dataset(self) -> None:
        x_train, x_test, y_train, y_test = train_test_split(
            self.dataset.values, self.desired_output, test_size=0.30)

        x_validation, x_test, y_validation, y_test = train_test_split(
            x_test, y_test, test_size=0.10)
        self.splitted_dataset = {'x_train': x_train, 'y_train': list(y_train), 'x_validation': x_validation,
                                 'y_validation': list(y_validation), 'x_test': x_test, 'y_test': list(y_test)}

    def map_out_desired_output(self) -> pd.Series:
        desired_outputs_label = self.dataset.iloc[:, -1].unique()

        mapping = {value: np.eye(len(desired_outputs_label))[
            index] for index, value in enumerate(desired_outputs_label)}

        self.desired_output_mapped = mapping
        desired_output = self.dataset.iloc[:, -1].apply(lambda x: mapping[x])
        self.desired_output = desired_output


# def main():
#     neural_network_caller = ctypes.CDLL(
#         '../shared_object/neuralNetworkCaller.so')
#     input = read_csv_file('./Iris.csv')
#     preprocessing = DataProcessing(input, neural_network_caller)
#     preprocessing.start_preprocessing(False, False)
    
#     preprocessing.train_dataset()


# main()
