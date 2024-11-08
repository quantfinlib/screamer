import numpy as np
import pandas as pd
import scipy.special

class Power_numpy:
    def __init__(self, p):
        self.p = p

    def __call__(self, x):
        return np.power(x, self.p)

class Power_pandas:
    def __init__(self, p):
        self.p = p

    def __call__(self, x):
        return pd.Series(x).pow(self.p).to_numpy()

class Power_scipy:
    def __init__(self, p):
        self.p = p

    def __call__(self, x):
        return scipy.special.pow(x, self.p)
