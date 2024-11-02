import pandas as pd
import numpy as np

class RollingSkew_pandas:
    def __init__(self, window_size):
        self.window_size = window_size

    def __call__(self, array):
        return pd.Series(array).rolling(window=self.window_size).skew().to_numpy()

class RollingSkew_numpy:
    def __init__(self, window_size):
        self.window_size = window_size

    def __call__(self, array):
        windowed_array = np.lib.stride_tricks.sliding_window_view(array, self.window_size)
        mean = np.mean(windowed_array, axis=-1)
        std = np.std(windowed_array, axis=-1, ddof=1)
        skewness = np.mean(((windowed_array - mean[:, None]) / std[:, None])**3, axis=-1)
        skewness *= self.window_size * self.window_size / (self.window_size - 1) / (self.window_size - 2)
        return np.concatenate((np.full(self.window_size - 1, np.nan), skewness))
