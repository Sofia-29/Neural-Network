import ctypes
import csv
import pandas as pd

def read_csv_file(name : str) -> pd.DataFrame:
    dataframe = pd.read_csv(name)

    return dataframe

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

    if len(dataset) <= 10:
        fill_results_list(dataset, 1, labels, sse)
    elif len(dataset) > 10 and len(dataset) <= 100:
        fill_results_list(dataset, 10, labels, sse)
    elif len(dataset) > 100 and len(dataset) <= 1000:
        fill_results_list(dataset, 100, labels, sse)
    elif len(dataset) > 1000 and len(dataset) <= 5000:
        fill_results_list(dataset, 500, labels, sse)
    else:
        fill_results_list(dataset, 1000, labels, sse)

    sse.append(dataset[-1])
    labels.append(len(dataset))
    return { "sse": sse, "labels": labels, "seconds": seconds }


def fill_results_list(dataset : list, step : int, labels : list, sse : list):
    for i in range(0, len, step):
        labels.append(i)
        subList = dataset[i:i+1000]
        middle_value = subList[len(subList)//2] # Aquí se obtiene el dato que está en la posición media de la sublista
        sse.append(middle_value)



def fill_pointer_list(ponter_list: ctypes.POINTER, list: list,
                    rows_count: int, columns_count: int) -> None:
    
    for index in range(rows_count):
        ponter_list[index] = (ctypes.c_float * columns_count)(*list[index])
    return