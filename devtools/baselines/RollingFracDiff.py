import numpy as np
import pandas as pd


def compute_weights(frac_order, window_size, threshold):
    weights = [1.0]
    for k in range(1, window_size):
        for k in range(1, window_size):
            weight = -weights[-1] * (frac_order - k + 1) / k
            if abs(weight) < threshold:
                break
            weights.append(weight)
        return np.array(weights)

class FracDiffNumpy:
    def __init__(self, frac_order, window_size, threshold=1e-5):
        self.frac_order = frac_order
        self.window_size = window_size
        self.threshold = threshold
        self.weights = compute_weights(frac_order, window_size, threshold)

    def compute_weights(self):
        weights = [1.0]
        for k in range(1, self.window_size):
            weight = -weights[-1] * (self.frac_order - k + 1) / k
            if abs(weight) < self.threshold:
                break
            weights.append(weight)
        return np.array(weights)

    def __call__(self, x):
        if len(x) < len(self.weights):
            raise ValueError("Input array length must be at least as large as the weights length.")
        return np.convolve(x, self.weights[::-1], mode="valid")




class FracDiffPandas:
    def __init__(self, frac_order, window_size, threshold=1e-5):
        self.frac_order = frac_order
        self.window_size = window_size
        self.threshold = threshold
        self.weights = compute_weights(frac_order, window_size, threshold)

    def compute_weights(self):
        weights = [1.0]
        for k in range(1, self.window_size):
            weight = -weights[-1] * (self.frac_order - k + 1) / k
            if abs(weight) < self.threshold:
                break
            weights.append(weight)
        return np.array(weights)

    def __call__(self, x):
        series = pd.Series(x)
        return series.rolling(window=self.window_size).apply(
            lambda window: np.dot(window[::-1], self.weights[:len(window)]), raw=True
        ).to_numpy()
