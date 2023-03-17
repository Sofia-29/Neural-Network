import ctypes
import csv
import pandas as pd
from collections import Counter


def read_csv_file(name: str) -> pd.DataFrame:
    dataframe = pd.read_csv(name)
    return dataframe


def analyze_results_test_model(name: str, desired_output_mapped: dict, desired_output: list):
    dataset = read_csv_file(name)
    dataset = dataset.dropna(axis=1)
    dataset = dataset.values.tolist()
    map_out_dataset = []
    incorrect_predictions = 0
    for index in range(len(dataset)):
        predicted_category = ""
        desired_category = ""
        for key, value in desired_output_mapped.items():
            value_list = list(value)
            if value_list == list(dataset[index]):
                predicted_category = key
            if value_list == list(desired_output[index]):
                desired_category = key
        
        map_out_dataset.append(predicted_category)

        if predicted_category != desired_category:
            incorrect_predictions += 1

    counter = Counter(map_out_dataset)
    dataLabels = []
    data = []
    for element, amount in counter.items():
        if amount > 1:
            dataLabels.append(element)
            data.append(amount)

    return {"dataTest": data, "dataLabelsTest": dataLabels, "incorrectPredictions": incorrect_predictions}


def read_training_errors_csv_file(name: str):
    dataset = []

    with open(name, "r") as resultsFile:
        data = csv.reader(resultsFile, delimiter=',', quotechar='"')
        for row in data:
            row = [float(sse) for sse in row if len(sse) > 2]
            dataset.extend(row)

    seconds = dataset[-1]
    dataset.pop()
    sse = []
    labels = []
    if len(dataset) <= 30:
        fill_results_list(dataset, 1, labels, sse)
    elif len(dataset) > 30 and len(dataset) <= 100:
        fill_results_list(dataset, 10, labels, sse)
    elif len(dataset) > 100 and len(dataset) <= 1000:
        fill_results_list(dataset, 100, labels, sse)
    elif len(dataset) > 1000 and len(dataset) <= 5000:
        fill_results_list(dataset, 500, labels, sse)
    else:
        fill_results_list(dataset, 1000, labels, sse)

    sse.append(dataset[-1])
    labels.append(len(dataset))
    return {"sse": sse, "labels": labels, "seconds": seconds}


def fill_results_list(dataset: list, step: int, labels: list, sse: list):
    for index in range(0, len(dataset), step):
        labels.append(index)
        value = dataset[index]
        if step != 1:
            subList = dataset[index:index+step]
            value = subList[len(subList)//2]
        sse.append(value)


def fill_pointer_list(ponter_list: ctypes.POINTER, list: list,
                      rows_count: int, columns_count: int) -> None:

    for index in range(rows_count):
        ponter_list[index] = (ctypes.c_float * columns_count)(*list[index])
    return
