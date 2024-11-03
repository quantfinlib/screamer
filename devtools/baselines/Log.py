import numpy as np
import pandas as pd
import scipy

class Log_numpy:
    def __call__(self, x):
        return np.log(x)

class Log_pandas:
    def __call__(self, x):
        return pd.Series(x).apply(np.log).to_numpy()
