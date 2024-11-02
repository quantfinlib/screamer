import pandas as pd
import numpy as np

class RollingZscore_pandas:
    def __init__(self, window_size):
        self.window_size = window_size

    def __call__(self, array):
        r = pd.Series(array).rolling(window=self.window_size)
        m = r.mean().to_numpy()
        s = r.std(ddof=1).to_numpy()
        return ((array - m) / s)

class RollingZscore_numpy:
    def __init__(self, window_size):
        self.window_size = window_size

    def __call__(self, array):
        windowed_array = np.lib.stride_tricks.sliding_window_view(array, self.window_size)
        mean = np.mean(windowed_array, axis=-1)
        std = np.std(windowed_array, axis=-1, ddof=1)
        ans = (windowed_array[:, -1] - mean) / std
        return np.concatenate((np.full(self.window_size - 1, np.nan), ans))
