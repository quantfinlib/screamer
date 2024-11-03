import numpy as np
import pandas as pd
from scipy.linalg import lstsq


class RollingPoly2_numpy:
    def __init__(self, window_size, derivative_order=0):
        self.window_size = window_size
        self.derivative_order = derivative_order

    def __call__(self, array):
        # Create a sliding window view of the input array
        windowed_array = np.lib.stride_tricks.sliding_window_view(array, self.window_size)

        # Generate x-values for polynomial fitting
        x = np.arange(self.window_size)

        # Fit a second-degree polynomial to each window
        coefficients = np.polyfit(x, windowed_array.T, 2)
        a, b, c = coefficients

        # Pad the beginning of the result arrays with NaNs to match the input length
        a = np.concatenate((np.full(self.window_size - 1, np.nan), a))
        b = np.concatenate((np.full(self.window_size - 1, np.nan), b))
        c = np.concatenate((np.full(self.window_size - 1, np.nan), c))

        # Compute the polynomial endpoints and their derivatives
        endpoint = a * (self.window_size - 1)**2 + b * (self.window_size - 1) + c
        endpoint_d = 2 * a * (self.window_size - 1) + b
        endpoint_d2 = 2 * a

        # Return the appropriate result based on the derivative order
        if self.derivative_order == 0:
            return endpoint
        elif self.derivative_order == 1:
            return endpoint_d
        elif self.derivative_order == 2:
            return endpoint_d2
        else:
            raise ValueError("Invalid derivative_order. Supported values are 0, 1, or 2.")


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

