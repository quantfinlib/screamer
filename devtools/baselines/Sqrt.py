import numpy as np
import pandas as pd
import scipy

class Sqrt_numpy:
    def __call__(self, x):
        return np.sqrt(x)

class Sqrt_pandas:
    def __call__(self, x):
        return pd.Series(x).apply(np.sqrt).to_numpy()

class Sqrt_scipy:
    def __call__(self, x):
        return scipy.sqrt(x)
