import numpy as np
import pandas as pd

class EwVar_pandas:
    def __init__(self, com=None, span=None, halflife=None, alpha=None):
        self.com = com
        self.span = span
        self.halflife = halflife
        self.alpha = alpha

    def __call__(self, x):
        return pd.Series(x).ewm(com=self.com, span=self.span, halflife=self.halflife, alpha=self.alpha).var().to_numpy()
