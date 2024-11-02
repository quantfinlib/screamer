import numpy as np
import pandas as pd

class Diff_numpy:
    def __init__(self, window_size):
        if window_size <= 0:
            raise ValueError("Window size must be positive.")
        self.window_size = window_size

    def __call__(self, array):
        result = np.zeros_like(array)
        if len(array) > self.window_size:
            result[self.window_size:] = array[self.window_size:] - array[:-self.window_size]
        else:
            result[:] = array  # No difference can be calculated if size <= window_size
        return result

class Diff_numpy_advanced:
    def __init__(self, window_size):
        if window_size <= 0:
            raise ValueError("Window size must be positive.")
        self.window_size = window_size

    def __call__(self, array):
        array = np.array(array)
        if len(array) <= self.window_size:
            return array  # Direct copy as differences can't be calculated
        
        padded_array = np.concatenate((np.zeros(self.window_size), array[self.window_size:]))
        result = array - np.roll(padded_array, self.window_size)
        result[:self.window_size] = array[:self.window_size]  # Retain initial values as is
        return result

class Diff_pandas:
    def __init__(self, window_size):
        if window_size <= 0:
            raise ValueError("Window size must be positive.")
        self.window_size = window_size

    def __call__(self, array):
        series = pd.Series(array)
        result = series.diff(periods=self.window_size).fillna(series)
        return result.to_numpy()
