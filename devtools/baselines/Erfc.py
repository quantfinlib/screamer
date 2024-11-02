import numpy as np
import pandas as pd
import scipy.special

class Erfc_numpy:
    def __call__(self, x):
        return np.vectorize(scipy.special.erfc)(x)

class Erfc_pandas:
    def __call__(self, x):
        return pd.Series(x).apply(scipy.special.erfc).to_numpy()

class Erfc_scipy:
    def __call__(self, x):
        return scipy.special.erfc(x)
