import numpy as np
import pandas as pd

class Softsign_numpy:
    def __call__(self, x):
        return x / (1 + np.abs(x))

class Softsign_pandas:
    def __call__(self, x):
        return pd.Series(x).apply(lambda v: v / (1 + np.abs(v))).to_numpy()
