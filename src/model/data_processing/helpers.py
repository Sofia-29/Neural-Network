import ctypes
import csv
import pandas as pd

# def read_results_csv_file(name : str) -> pd.DataFrame:
#     dataframe = pd.read_csv(name, header=None, sep=',')
    
#     return dataframe

def read_results_csv_file(name : str):
    sum_of_squared_errors = []
    with open(name, "r") as resultsFile:
        data = csv.reader(resultsFile, delimiter=',', quotechar='"')
        for row in data:
            row = [float(sse) for sse in row if len(sse) > 2]
            sum_of_squared_errors.extend(row)

    seconds  = sum_of_squared_errors[-1]
    sum_of_squared_errors.pop()
    labels = [ index for index in range(1, len(sum_of_squared_errors) + 1)]
    return { "sse": sum_of_squared_errors, "labels": labels, "seconds": seconds }

def fill_pointer_list(ponter_list: ctypes.POINTER, list: list,
                    rows_count: int, columns_count: int) -> None:
    
    for index in range(rows_count):
        ponter_list[index] = (ctypes.c_float * columns_count)(*list[index])
    return