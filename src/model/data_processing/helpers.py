import ctypes
import csv
import pandas as pd

# def read_results_csv_file(name : str) -> pd.DataFrame:
#     dataframe = pd.read_csv(name, header=None, sep=',')
    
#     return dataframe

def read_results_csv_file(name : str):
    dataset = []
    with open(name, "r") as resultsFile:
        data = csv.reader(resultsFile, delimiter=',', quotechar='"')
        for row in data:
            row = [float(sse) for sse in row if len(sse) > 2]
            dataset.extend(row)
    seconds  = dataset[-1]
    dataset.pop()
    sse = []
    labels = []
    for i in range(0, len(dataset), 1000):
        labels.append(i)
        subList = dataset[i:i+1000]
        middle_value = subList[len(subList)//2] # Aquí se obtiene el dato que está en la posición media de la sublista
        sse.append(middle_value)
    sse.append(dataset[-1])
    labels.append(len(dataset))
    return { "sse": sse, "labels": labels, "seconds": seconds }

def fill_pointer_list(ponter_list: ctypes.POINTER, list: list,
                    rows_count: int, columns_count: int) -> None:
    
    for index in range(rows_count):
        ponter_list[index] = (ctypes.c_float * columns_count)(*list[index])
    return