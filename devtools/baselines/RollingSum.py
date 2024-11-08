import pandas as pd
import numpy as np

class RollingSum_pandas:
    def __init__(self, window_size):
        self.window_size = window_size

    def __call__(self, array):
        return pd.Series(array).rolling(window=self.window_size).sum().to_numpy()

class RollingSum_numpy:
    def __init__(self, window_size):
        self.window_size = window_size

    def __call__(self, array):
        ans = np.cumsum(array)
        ans[self.window_size:] = ans[self.window_size:] - ans[:-self.window_size]
        return np.concatenate((np.full(self.window_size - 1, np.nan), ans[self.window_size - 1:]))
