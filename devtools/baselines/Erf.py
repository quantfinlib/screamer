import numpy as np
import pandas as pd
import scipy.special

class Erf_numpy:
    def __call__(self, x):
        return np.vectorize(scipy.special.erf)(x)

class Erf_pandas:
    def __call__(self, x):
        return pd.Series(x).apply(scipy.special.erf).to_numpy()

class Erf_scipy:
    def __call__(self, x):
        return scipy.special.erf(x)