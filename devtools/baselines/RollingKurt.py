import pandas as pd
import numpy as np

class RollingKurt_pandas:
    def __init__(self, window_size):
        self.window_size = window_size

    def __call__(self, array):
        return pd.Series(array).rolling(window=self.window_size).kurt().to_numpy()

class RollingKurt_numpy:
    def __init__(self, window_size):
        self.window_size = window_size

    def __call__(self, array):
        N = self.window_size
        windowed_array = np.lib.stride_tricks.sliding_window_view(array, N)

        mean = np.mean(windowed_array, axis=1)
        std = np.std(windowed_array, axis=1, ddof=1)
        sum_xx = np.sum(windowed_array**2, axis=1)
        sum_xxx = np.sum(windowed_array**3, axis=1)
        sum_xxxx = np.sum(windowed_array**4, axis=1)

        m4 = sum_xxxx - 4 * mean * sum_xxx + 6 * mean**2 * sum_xx - 3 * N * mean**4
        numerator = N * (N + 1) * m4
        denominator = (N - 1) * (N - 2) * (N - 3) * std**4
        adjustment = (3 * (N - 1)**2) / ((N - 2) * (N - 3))
        kurtosis = numerator / denominator - adjustment

        return np.concatenate((np.full(self.window_size - 1, np.nan), kurtosis))
