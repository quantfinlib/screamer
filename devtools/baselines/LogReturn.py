import numpy as np
import pandas as pd

class LogReturn_numpy:
    def __init__(self, window_size):
        if window_size <= 0:
            raise ValueError("Window size must be positive.")
        self.window_size = window_size

    def __call__(self, array):
        array = np.array(array, dtype=np.float64)
        result = np.full_like(array, np.nan)
        if len(array) > self.window_size:
            result[self.window_size:] = np.log(array[self.window_size:] / array[:-self.window_size])
        return result

class LogReturn_numpy_vectorized:
    def __init__(self, window_size):
        if window_size <= 0:
            raise ValueError("Window size must be positive.")
        self.window_size = window_size

    def __call__(self, array):
        array = np.array(array, dtype=np.float64)
        if len(array) <= self.window_size:
            return np.full_like(array, np.nan)

        result = np.empty(len(array))
        result[:self.window_size] = np.nan  # First `window_size` elements are NaN
        result[self.window_size:] = np.log(array[self.window_size:] / array[:-self.window_size])
        return result

class LogReturn_pandas:
    def __init__(self, window_size):
        if window_size <= 0:
            raise ValueError("Window size must be positive.")
        self.window_size = window_size

    def __call__(self, array):
        series = pd.Series(array)
        result = np.log(series / series.shift(self.window_size))
        return result.to_numpy()
