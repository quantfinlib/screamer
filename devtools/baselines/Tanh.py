import numpy as np
import pandas as pd
import scipy

class Tanh_numpy:
    def __call__(self, x):
        return np.tanh(x)

class Tanh_pandas:
    def __call__(self, x):
        return pd.Series(x).apply(np.tanh).to_numpy()

class Tanh_scipy:
    def __call__(self, x):
        return scipy.tanh(x)
