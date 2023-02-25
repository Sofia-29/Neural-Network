import sys
sys.path.append('..')

from model.data_processing import DataProcessing

import pandas as pd
from fastapi import APIRouter, File, UploadFile

router = APIRouter()
data_processing = None

@router.post("/split-dataset")
def split_dataset(dataset: UploadFile = File(...), normalize_data: bool = False,
                  map_desired_output: bool = False):
    try:
        csv_reader = pd.read_csv(dataset.file)
        data_processing = DataProcessing(csv_reader)
        data_processing.start_preprocessing()
    except Exception:
        return {"response": "Error"}
    finally:
        dataset.file.close()
    return {"response": "Ok"}


@router.post("/train-dataset")
def train_dataset():
    try:
       data_processing.train_dataset()
    except Exception:
        return {"response": "Error"}
    return {"response": "Ok"}
