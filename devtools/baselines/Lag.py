import numpy as np
import pandas as pd

class Lag_numpy:
    def __init__(self, window_size):
        if window_size <= 0:
            raise ValueError("Window size must be positive.")
        self.window_size = window_size

    def __call__(self, array):
        result = np.zeros_like(array)
        if len(array) > self.window_size:
            result[self.window_size:] = array[:-self.window_size]
        return result

class Lag_numpy_manual:
    def __init__(self, window_size):
        if window_size <= 0:
            raise ValueError("Window size must be positive.")
        self.window_size = window_size

    def __call__(self, array):
        array = np.array(array)
        if len(array) <= self.window_size:
            return np.zeros_like(array)
        
        lagged_array = np.zeros(len(array))
        lagged_array[self.window_size:] = array[:-self.window_size]
        return lagged_array

class Lag_pandas:
    def __init__(self, window_size):
        if window_size <= 0:
            raise ValueError("Window size must be positive.")
        self.window_size = window_size

    def __call__(self, array):
        series = pd.Series(array)
        result = series.shift(periods=self.window_size, fill_value=0)
        return result.to_numpy()
