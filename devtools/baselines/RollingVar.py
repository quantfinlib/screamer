import pandas as pd
import numpy as np

class RollingVar_pandas:
    def __init__(self, window_size):
        self.window_size = window_size

    def __call__(self, array):
        return pd.Series(array).rolling(window=self.window_size).var().to_numpy()

class RollingVar_numpy:
    def __init__(self, window_size):
        self.window_size = window_size

    def __call__(self, array):
        windowed_array = np.lib.stride_tricks.sliding_window_view(array, self.window_size)
        ans = np.var(windowed_array, axis=-1, ddof=1)
        return np.concatenate((np.full(self.window_size - 1, np.nan), ans))
