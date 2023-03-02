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
async def split_dataset(dataset: UploadFile = File(...), normalize_data: bool = Form(...), map_desired_output: bool = Form(...), ):
    try:
        # contents = None
        async with dataset:
            contents = await dataset.read()
        # print(contents)
        dataframe = pd.read_csv(contents.decode())
        #data_processing = DataProcessing(dataframe)

        # print(normalize_data)
        # print(map_desired_output)
        #data_processing.start_preprocessing(normalize_data, map_desired_output)
    except Exception:
        return {"response": "Error"}
    finally:
        dataset.file.close()
    return {"response": "Ok"}


@app.get("/train-dataset")
async def train_dataset():
    try:
       data_processing.train_dataset()
    except Exception:
        return {"response": "Error"}
    return {"response": "Ok"}
