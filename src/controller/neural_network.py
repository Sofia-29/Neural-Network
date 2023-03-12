import sys
sys.path.append('..')
from model.data_processing.data_processing import DataProcessing

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

@app.post("/split-dataset")
async def split_dataset(file: UploadFile = File(...), normalize_data: bool = Form(...),
                        map_desired_output: bool = Form(...), split_dataset: bool = Form(...),):
    try:
        dataset = pd.read_csv(file.file)
        data_processing.start_preprocessing(dataset=dataset, normalize=normalize_data, map_desired_output=map_desired_output, split_dataset=split_dataset)
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
