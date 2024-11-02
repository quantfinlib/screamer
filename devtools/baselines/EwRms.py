import numpy as np
import pandas as pd

class EwRms_pandas:
    def __init__(self, com=None, span=None, halflife=None, alpha=None):
        self.com = com
        self.span = span
        self.halflife = halflife
        self.alpha = alpha

    def __call__(self, x):
        mean_squared = pd.Series(x).ewm(com=self.com, span=self.span, halflife=self.halflife, alpha=self.alpha).mean().pow(2)
        return mean_squared.pow(0.5).to_numpy()
