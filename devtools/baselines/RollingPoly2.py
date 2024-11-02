import numpy as np
import pandas as pd
from scipy.linalg import lstsq

class RollingPoly2_numpy:
    def __init__(self, window_size, derivative_order=0):
        self.window_size = window_size
        self.derivative_order = derivative_order
        self.x = np.arange(window_size)
        self.Sx = np.sum(self.x)
        self.Sxx = np.sum(self.x ** 2)
        self.Sxxx = np.sum(self.x ** 3)
        self.Sxxxx = np.sum(self.x ** 4)
        self.N = window_size

        # Precompute the necessary terms
        self.Zxx = self.Sxx - self.Sx * self.Sx / self.N
        self.Zxx2 = self.Sxxx - self.Sxx * self.Sx / self.N
        self.Zx2x2 = self.Sxxxx - self.Sxx * self.Sxx / self.N
        self.d = self.Zxx * self.Zx2x2 - self.Zxx2 ** 2

    def __call__(self, array):
        if len(array) < self.window_size:
            raise ValueError("Input array length must be at least the window size")

        windows = np.lib.stride_tricks.sliding_window_view(array, self.window_size)
        endpoints = []
        slopes = []
        curvatures = []

        for window in windows:
            Sy = np.sum(window)
            Sxy = np.sum(window * self.x)
            Sxxy = np.sum(window * self.x ** 2)

            Zxy = Sxy - self.Sx * Sy / self.N
            Zx2y = Sxxy - self.Sxx * Sy / self.N

            # Solve for coefficients a, b, c
            a = (Zx2y * self.Zxx - Zxy * self.Zxx2) / self.d
            b = (Zxy * self.Zx2x2 - Zx2y * self.Zxx2) / self.d
            c = Sy / self.N - b * self.Sx / self.N - a * self.Sxx / self.N

            # Compute endpoint, slope, and curvature
            endpoint = a * (self.N - 1) ** 2 + b * (self.N - 1) + c
            slope = 2 * a * (self.N - 1) + b
            curvature = 2 * a

            endpoints.append(endpoint)
            slopes.append(slope)
            curvatures.append(curvature)

        if self.derivative_order == 0:
            return np.array(endpoints)
        elif self.derivative_order == 1:
            return np.array(slopes)
        elif self.derivative_order == 2:
            return np.array(curvatures)

class RollingPoly2_scipy:
    def __init__(self, window_size, derivative_order=0):
        self.window_size = window_size
        self.derivative_order = derivative_order
        self.x = np.arange(window_size).reshape(-1, 1)

    def __call__(self, array):
        if len(array) < self.window_size:
            raise ValueError("Input array length must be at least the window size")

        windows = np.lib.stride_tricks.sliding_window_view(array, self.window_size)
        results = {'endpoint': [], 'slope': [], 'curvature': []}

        for window in windows:
            A = np.hstack([self.x ** 2, self.x, np.ones_like(self.x)])
            coeffs = lstsq(A, window)[0]  # Coefficients [a, b, c]

            a, b, c = coeffs
            endpoint = a * (self.window_size - 1) ** 2 + b * (self.window_size - 1) + c
            slope = 2 * a * (self.window_size - 1) + b
            curvature = 2 * a

            results['endpoint'].append(endpoint)
            results['slope'].append(slope)
            results['curvature'].append(curvature)

        return (
            np.array(results['endpoint']) if self.derivative_order == 0 else
            np.array(results['slope']) if self.derivative_order == 1 else
            np.array(results['curvature'])
        )

class RollingPoly2_pandas:
    def __init__(self, window_size, derivative_order=0):
        self.window_size = window_size
        self.derivative_order = derivative_order

    def __call__(self, array):
        series = pd.Series(array)
        fit_results = series.rolling(window=self.window_size).apply(
            lambda window: np.polyfit(
                np.arange(len(window)), window, 2
            )[0 if self.derivative_order == 2 else 1 if self.derivative_order == 1 else 2] +
            (2 * np.polyfit(np.arange(len(window)), window, 2)[0] * (self.window_size - 1) +
             np.polyfit(np.arange(len(window)), window, 2)[1] if self.derivative_order == 1 else
             np.polyfit(np.arange(len(window)), window, 2)[0] * (self.window_size - 1) ** 2 +
             np.polyfit(np.arange(len(window)), window, 2)[1] * (self.window_size - 1)
             if self.derivative_order == 0 else 0),
            raw=True
        )
        return fit_results.to_numpy()
