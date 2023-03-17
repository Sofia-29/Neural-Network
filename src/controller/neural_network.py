import sys
sys.path.append('..')
from model.data_processing.data_processing import DataProcessing
from model.data_processing.helpers import read_training_errors_csv_file, analyze_results_test_model

import ctypes
import pandas as pd
from fastapi import FastAPI, File, UploadFile, Form
from fastapi.middleware.cors import CORSMiddleware


app = FastAPI()
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

neural_network_caller = ctypes.CDLL(
    '../model/shared_object/neuralNetworkCaller.so')
data_processing = DataProcessing(neural_network_caller=neural_network_caller)


@app.post("/process-data")
async def process_data(file: UploadFile = File(...), normalize_data: bool = Form(...),
                        map_desired_output: bool = Form(...), split_dataset: bool = Form(...),):
    try:
        dataset = pd.read_csv(file.file)
        data_processing.start_preprocessing(
            dataset=dataset, normalize=normalize_data, map_desired_output=map_desired_output, split_dataset=split_dataset)
    except Exception:
        return {"response": "Error"}
    finally:
        file.file.close()
    return {"response": "Ok"}


@app.get("/train-dataset")
def train_dataset():
    try:
        data_processing.train_dataset()
    except Exception:
        return {"response": "Error"}
    return {"response": "Ok"}


@app.get("/test-model")
def test_model():
    try:
        if data_processing.user_decision:
            data_processing.test_model()
    except Exception:
        return {"response": "Error"}
    return {"response": "Ok"}


@app.get("/results")
def get_results():
    response = {}
    try:
        result_training_data = read_training_errors_csv_file(
            "../model/results_files/training_errors/training_errors.csv")
        data = result_training_data["sse"]
        labels = result_training_data["labels"]
        seconds = result_training_data["seconds"]
        if data_processing.user_decision:
            validation_data = analyze_results_test_model("../model/results_files/predictions/validation_predictions.csv",
                                                         data_processing.desired_output_mapped, data_processing.splitted_dataset["y_validation"])

            test_data = analyze_results_test_model("../model/results_files/predictions/tests_predictions.csv",
                                                   data_processing.desired_output_mapped, data_processing.splitted_dataset["y_test"])
            response = {"response": "Ok", "userDecision": data_processing.user_decision, "trainingData": data, "trainingLabels": labels,
                        "seconds": str(seconds), "validationData": validation_data["dataTest"],
                        "validationLabels": validation_data["dataLabelsTest"],
                        "validationErrors": validation_data["incorrectPredictions"],
                        "testData": test_data["dataTest"],
                        "testLabels": test_data["dataLabelsTest"],
                        "testErrors": test_data["incorrectPredictions"]}
        else:
            response = {"response": "Ok", "userDecision": data_processing.user_decision, "trainingData": data, "trainingLabels": labels,
                        "seconds": str(seconds)}

    except Exception:
       response = {"response": "Error"}
    return response
