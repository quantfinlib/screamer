import numpy as np
import pandas as pd

def compute_partial_sum(length, a):
    w = np.zeros(length)
    w[0] = 1.0
    # Compute partial sums iteratively
    for i in range(1, length):
        w[i] = w[i - 1] * a + 1.0
    return w

def compute_n_eff(length, alpha):
    w = compute_partial_sum(length, 1-alpha)
    w2 = compute_partial_sum(length, (1-alpha)**2)
    n_eff = (w ** 2) / w2
    return n_eff

class EwKurt_pandas:
    def __init__(self, com=None, span=None, halflife=None, alpha=None):
        self.com = com
        self.span = span
        self.halflife = halflife
        self.alpha = alpha

    def __call__(self, x):
        series = pd.Series(x)
        length = len(series)

        # Determine alpha
        if self.alpha is not None:
            alpha = self.alpha
        elif self.com is not None:
            alpha = 1 / (1 + self.com)
        elif self.span is not None:
            alpha = 2 / (self.span + 1)
        elif self.halflife is not None:
            alpha = 1 - np.exp(-np.log(2) / self.halflife)
        else:
            raise ValueError("One of com, span, halflife, or alpha must be provided.")

        # Compute effectie number of samplee
        n_eff = compute_n_eff(length, alpha)

        # Calculate the weighted mean and standard deviation
        ewm_mean = series.ewm(com=self.com, span=self.span, halflife=self.halflife, alpha=self.alpha).mean()
        ewm_std = series.ewm(com=self.com, span=self.span, halflife=self.halflife, alpha=self.alpha).std()

        # Compute the fourth central moment (m4)
        ewm_quart_diff = ((series - ewm_mean) / ewm_std) ** 4
        m4 = ewm_quart_diff.ewm(com=self.com, span=self.span, halflife=self.halflife, alpha=self.alpha).mean()

        # Bias correction for kurtosis (excess kurtosis)
        kurt = ((n_eff * (n_eff + 1) * m4) / ((n_eff - 1) * (n_eff - 2) * (n_eff - 3))) - (3 * (n_eff - 1) ** 2 / ((n_eff - 2) * (n_eff - 3)))
        kurt = kurt - 3  # Convert to excess kurtosis
        kurt[n_eff <= 3] = np.nan  # Handle cases where n_eff is too small

        return kurt.to_numpy()
