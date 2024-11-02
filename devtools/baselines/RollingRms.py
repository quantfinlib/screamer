import numpy as np
import pandas as pd

class RollingRms_numpy:
    def __init__(self, window_size):
        if window_size <= 0:
            raise ValueError("Window size must be positive.")
        self.window_size = window_size

    def __call__(self, array):
        array = np.array(array, dtype=np.float64)
        result = np.full_like(array, np.nan)
        if len(array) >= self.window_size:
            squared_array = array ** 2
            rolling_sum = np.convolve(squared_array, np.ones(self.window_size), 'valid') / self.window_size
            result[self.window_size - 1:] = np.sqrt(rolling_sum)
        return result

class RollingRms_numpy_strided:
    def __init__(self, window_size):
        if window_size <= 0:
            raise ValueError("Window size must be positive.")
        self.window_size = window_size

    def __call__(self, array):
        if len(array) < self.window_size:
            return np.full_like(array, np.nan)

        windowed_array = np.lib.stride_tricks.sliding_window_view(array, self.window_size)
        rolling_rms = np.sqrt(np.mean(windowed_array ** 2, axis=-1))

        result = np.full(len(array), np.nan)
        result[self.window_size - 1:] = rolling_rms
        return result

class RollingRms_pandas:
    def __init__(self, window_size):
        if window_size <= 0:
            raise ValueError("Window size must be positive.")
        self.window_size = window_size

    def __call__(self, array):
        series = pd.Series(array)
        result = series.rolling(window=self.window_size).apply(lambda x: np.sqrt(np.mean(x ** 2)), raw=True)
        return result.to_numpy()
