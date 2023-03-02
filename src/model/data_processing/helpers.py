import ctypes
import pandas as pd

def read_csv_file(name : str) -> pd.DataFrame:
    dataframe = pd.read_csv(name, header=None, sep=',')
    return dataframe

def fill_pointer_list(ponter_list: ctypes.POINTER, list: list,
                    rows_count: int, columns_count: int) -> None:
    for i in range(rows_count):
        ponter_list[i] = (ctypes.c_float * columns_count)(*list[i])
    return