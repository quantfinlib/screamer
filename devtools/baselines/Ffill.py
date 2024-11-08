import numpy as np
import pandas as pd

class Ffill_pandas:
    def __call__(self, x):
        return pd.Series(x).ffill().to_numpy()
