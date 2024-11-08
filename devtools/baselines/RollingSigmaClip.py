import numpy as np
import pandas as pd

class RollingSigmaClip:
    def __init__(self, window_size, lower=None, upper=None, output=0):
        if window_size <= 0:
            raise ValueError("Window size must be positive.")
        if output not in [0, 1, 2, 3]:
            raise ValueError("Output order must be 0 (clipped value), 1 (mean), 2 (std), or 3 (clipped as NaN).")

        self.window_size = window_size
        self.lower = lower if lower is not None else -np.inf
        self.upper = upper if upper is not None else np.inf
        self.output = output

    def __call__(self, array):
        series = pd.Series(array)

        # Calculate rolling mean and standard deviation
        rolling_mean = series.rolling(window=self.window_size, min_periods=1).mean()
        rolling_std = series.rolling(window=self.window_size, min_periods=1).std()

        # Compute z-scores
        z_scores = (series - rolling_mean) / rolling_std

        # Clip values based on the bounds and output the appropriate result
        clipped_series = series.copy()
        clipped_series[z_scores < self.lower] = rolling_mean[z_scores < self.lower] + self.lower * rolling_std[z_scores < self.lower]
        clipped_series[z_scores > self.upper] = rolling_mean[z_scores > self.upper] + self.upper * rolling_std[z_scores > self.upper]

        if self.output == 0:
            return clipped_series.to_numpy()
        elif self.output == 1:
            return rolling_mean.to_numpy()
        elif self.output == 2:
            return rolling_std.to_numpy()
        elif self.output == 3:
            clipped_series[(z_scores < self.lower) | (z_scores > self.upper)] = np.nan
            return clipped_series.to_numpy()
