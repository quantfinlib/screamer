import numpy as np
import pandas as pd

class FillNa_numpy:
    def __init__(self, fill):
        self.fill = fill

    def __call__(self, x):
        return np.where(np.isnan(x), self.fill, x)

class FillNa_pandas:
    def __init__(self, fill):
        self.fill = fill

    def __call__(self, x):
        return pd.Series(x).fillna(self.fill).to_numpy()
