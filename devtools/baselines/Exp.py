import numpy as np
import pandas as pd
import scipy

class Exp_numpy:
    def __call__(self, x):
        return np.exp(x)

class Exp_pandas:
    def __call__(self, x):
        return pd.Series(x).apply(np.exp).to_numpy()
