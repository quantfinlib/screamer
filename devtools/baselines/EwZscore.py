import numpy as np
import pandas as pd

class EwZscore_pandas:
    def __init__(self, com=None, span=None, halflife=None, alpha=None):
        self.com = com
        self.span = span
        self.halflife = halflife
        self.alpha = alpha

    def __call__(self, x):
        ewm_mean = pd.Series(x).ewm(com=self.com, span=self.span, halflife=self.halflife, alpha=self.alpha).mean()
        ewm_std = pd.Series(x).ewm(com=self.com, span=self.span, halflife=self.halflife, alpha=self.alpha).std()
        return ((pd.Series(x) - ewm_mean) / ewm_std).to_numpy()
