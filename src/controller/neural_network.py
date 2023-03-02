import sys
sys.path.append('..')
from model.data_processing.data_processing import DataProcessing

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

data_processing = None

@app.post("/split-dataset")
async def split_dataset(file: UploadFile = File(...), normalize_data: bool = Form(...), map_desired_output: bool = Form(...), ):
    try:
        print(type(file))
        # dataset = pd.read_csv(file.file)
        # print(dataset)
        # data_processing = DataProcessing(dataset=dataset)
        # print(data_processing.dataset)
        # print()
        #data_processing.start_preprocessing(normalize=normalize_data, map_desired_output=map_desired_output)
        #print(data_processing.desired_output_mapped)

    except Exception:
        return {"response": "Error"}
    finally:
        file.file.close()
    return {"response": "Ok"}


@app.get("/train-dataset")
async def train_dataset():
    try:
       data_processing.train_dataset()
    except Exception:
        return {"response": "Error"}
    return {"response": "Ok"}
