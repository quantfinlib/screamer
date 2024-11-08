import numpy as np
import pandas as pd

class Relu_numpy:
    def __call__(self, array):
        return np.maximum(array, 0)

class Relu_numpy_vectorized:
    def __call__(self, array):
        array = np.array(array, dtype=np.float64)
        result = np.where(array > 0, array, 0)
        return result

class Relu_pandas:
    def __call__(self, array):
        series = pd.Series(array)
        result = series.apply(lambda x: max(x, 0))
        return result.to_numpy()
