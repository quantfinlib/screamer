import numpy as np
import pandas as pd

class Sigmoid_numpy:
    def __call__(self, x):
        return 1 / (1 + np.exp(-x))

class Sigmoid_pandas:
    def __call__(self, x):
        return pd.Series(x).apply(lambda v: 1 / (1 + np.exp(-v))).to_numpy()
