import numpy as np
import pandas as pd

class Sign_numpy:
    def __call__(self, x):
        return np.sign(x)

class Sign_pandas:
    def __call__(self, x):
        return pd.Series(x).apply(np.sign).to_numpy()
