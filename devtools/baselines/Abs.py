import numpy as np
import pandas as pd

class Abs_numpy:
    def __call__(self, x):
        return np.abs(x)

class Abs_pandas:
    def __call__(self, x):
        return pd.Series(x).abs().to_numpy()
