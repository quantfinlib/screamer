import numpy as np
import pandas as pd
from scipy.special import binom


def compute_weights(frac_order, window_size, threshold):
    weights = (-1) ** np.arange(window_size) * binom(frac_order, np.arange(window_size))
    weights = weights[abs(weights) >= threshold]
    return weights


class RollingFracDiff_numpy:
    def __init__(self, frac_order, window_size, threshold=1e-5):
        self.frac_order = frac_order
        self.window_size = window_size
        self.threshold = threshold
        self.weights = compute_weights(frac_order, window_size, threshold)

    def __call__(self, x):
        if len(x) < len(self.weights):
            raise ValueError("Input array length must be at least as large as the weights length.")
        return np.convolve(x, self.weights, mode="full")[:len(x)]


class RollingFracDiff_pandas:
    def __init__(self, frac_order, window_size, threshold=1e-5):
        self.frac_order = frac_order
        self.window_size = window_size
        self.threshold = threshold
        self.weights = compute_weights(frac_order, window_size, threshold)
    
    def __call__(self, array):
        if len(array) < len(self.weights):
            raise ValueError("Input Series length must be at least as large as the weights length.")
        
        def apply_weights(values):
            if len(values) < len(self.weights):
                return np.nan
            return np.dot(values[-len(self.weights):], self.weights[::-1])
        
        return pd.Series(array).rolling(
            window=len(self.weights), min_periods=len(self.weights)
        ).apply(apply_weights, raw=True).to_numpy()
