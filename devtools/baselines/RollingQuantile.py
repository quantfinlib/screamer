import pandas as pd
import numpy as np

class RollingQuantile_pandas:
    def __init__(self, window_size, quantile=0.75):
        self.window_size = window_size
        self.quantile = quantile

    def __call__(self, array):
        return pd.Series(array).rolling(window=self.window_size).quantile(self.quantile).to_numpy()

class RollingQuantile_numpy:
    def __init__(self, window_size, quantile=0.75):
        self.window_size = window_size
        self.quantile = quantile

    def __call__(self, array):
        windowed_array = np.lib.stride_tricks.sliding_window_view(array, self.window_size)
        ans = np.percentile(windowed_array, self.quantile * 100, axis=-1)
        return np.concatenate((np.full(self.window_size - 1, np.nan), ans))
