import numpy as np
import pandas as pd

class Selu_numpy:
    def __call__(self, x):
        alpha = 1.6732632423543772848170429916717
        scale = 1.0507009873554804934193349852946
        return scale * np.where(x >= 0, x, alpha * (np.exp(x) - 1))

class Selu_pandas:
    def __call__(self, x):
        alpha = 1.6732632423543772848170429916717
        scale = 1.0507009873554804934193349852946
        return pd.Series(x).apply(lambda v: scale * (v if v >= 0 else alpha * (np.exp(v) - 1))).to_numpy()
