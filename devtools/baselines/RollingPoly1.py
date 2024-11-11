import numpy as np
import pandas as pd
from scipy.linalg import lstsq

class RollingPoly1_numpy:
    def __init__(self, window_size, derivative_order=0):
        self.window_size = window_size
        self.derivative_order = derivative_order
        self.x = np.arange(window_size)
        self.sum_x = np.sum(self.x)
        self.n_var_x = np.var(self.x, ddof=0) * window_size

    def __call__(self, array):
        if len(array) < self.window_size:
            raise ValueError("Input array length must be at least the window size")
        array = np.concatenate((np.zeros(self.window_size -1), array))
        # Stride over the array to create rolling windows
        windows = np.lib.stride_tricks.sliding_window_view(array, self.window_size)
        sum_y = np.sum(windows, axis=-1)
        sum_xy = np.sum(windows * self.x, axis=-1)
        n_covar_xy = sum_xy - (self.sum_x * sum_y) / self.window_size

        slopes = n_covar_xy / self.n_var_x
        intercepts = (sum_y - slopes * self.sum_x) / self.window_size
        endpoints = intercepts + slopes * (self.window_size - 1)

        return endpoints if self.derivative_order == 0 else slopes

class RollingPoly1_scipy:
    def __init__(self, window_size, derivative_order=0):
        self.window_size = window_size
        self.derivative_order = derivative_order
        self.x = np.arange(window_size).reshape(-1, 1)

    def __call__(self, array):
        if len(array) < self.window_size:
            raise ValueError("Input array length must be at least the window size")

        # Stride over the array to create rolling windows
        windows = np.lib.stride_tricks.sliding_window_view(array, self.window_size)
        endpoints = []
        slopes = []

        for window in windows:
            # Fit a linear polynomial to the window using least squares
            A = np.hstack([self.x, np.ones_like(self.x)])
            result = lstsq(A, window)[0]
            slope, intercept = result[0], result[1]
            endpoint = intercept + slope * (self.window_size - 1)

            endpoints.append(endpoint)
            slopes.append(slope)

        endpoints = np.concatenate((np.full(self.window_size - 1, np.nan), endpoints))
        slopes = np.concatenate((np.full(self.window_size - 1, np.nan), slopes))

        return endpoints if self.derivative_order == 0 else slopes

class RollingPoly1_pandas:
    def __init__(self, window_size, derivative_order=0):
        self.window_size = window_size
        self.derivative_order = derivative_order

    def __call__(self, array):
        series = pd.Series(array)
        fit_results = series.rolling(window=self.window_size).apply(
            lambda window: np.polyfit(
                np.arange(len(window)), window, 1
            )[0 if self.derivative_order == 1 else 1] + 
            (np.polyfit(np.arange(len(window)), window, 1)[0] * (self.window_size - 1) if self.derivative_order == 0 else 0),
            raw=True
        )
        return fit_results.to_numpy()
