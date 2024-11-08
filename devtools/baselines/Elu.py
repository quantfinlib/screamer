import numpy as np
import pandas as pd

class Elu_numpy:
    def __call__(self, x):
        alpha = 1.0
        return np.where(x >= 0, x, alpha * (np.exp(x) - 1))

class Elu_pandas:
    def __call__(self, x):
        alpha = 1.0
        return pd.Series(x).apply(lambda v: v if v >= 0 else alpha * (np.exp(v) - 1)).to_numpy()
