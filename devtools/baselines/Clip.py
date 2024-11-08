import numpy as np
import pandas as pd

class Clip_numpy:
    def __init__(self, lower=None, upper=None):
        self.lower = lower if lower is not None else -np.inf
        self.upper = upper if upper is not None else np.inf

    def __call__(self, array):
        return np.clip(array, self.lower, self.upper)

class Clip_numpy_manual:
    def __init__(self, lower=None, upper=None):
        self.lower = lower if lower is not None else -np.inf
        self.upper = upper if upper is not None else np.inf

    def __call__(self, array):
        array = np.array(array)
        array[array < self.lower] = self.lower
        array[array > self.upper] = self.upper
        return array

class Clip_pandas:
    def __init__(self, lower=None, upper=None):
        self.lower = lower
        self.upper = upper

    def __call__(self, array):
        series = pd.Series(array)
        return series.clip(lower=self.lower, upper=self.upper).to_numpy()
