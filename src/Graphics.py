from ctypes.wintypes import FLOAT
import numpy as np
import matplotlib.pyplot as plt

def showGraphic(list):
    np.asarray(list, dtype=np.float32)
    plt.plot(list, color="blue", linewidth=2.5, linestyle="-")
    plt.title('Sum of Squared Errors')
    plt.show()