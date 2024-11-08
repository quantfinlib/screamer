import pandas as pd
import numpy as np

class RollingMedian_pandas:
    def __init__(self, window_size):
        self.window_size = window_size

    def __call__(self, array):
        return pd.Series(array).rolling(window=self.window_size).median().to_numpy()

class RollingMedian_numpy:
    def __init__(self, window_size):
        self.window_size = window_size

    def __call__(self, array):
        windowed_array = np.lib.stride_tricks.sliding_window_view(array, self.window_size)
        ans = np.median(windowed_array, axis=-1)
        return np.concatenate((np.full(self.window_size - 1, np.nan), ans))
