import numpy as np
from scipy.signal import lfilter, butter

class Butter2_scipy:
    def __init__(self, cutoff_freq, sample_rate=1.0):
        nyquist_freq = 0.5 * sample_rate  # Nyquist frequency based on the sample rate
        normalized_cutoff = cutoff_freq / nyquist_freq

        # Generate Butterworth filter coefficients for a second-order filter
        self.b, self.a = butter(2, normalized_cutoff, btype='low', analog=False)

    def __call__(self, array):
        # Apply the filter to the array using lfilter
        return lfilter(self.b, self.a, array)


class Butter2_numpy:
    def __init__(self, cutoff_freq, sample_rate=1.0):
        nyquist = 0.5 * sample_rate
        normalized_cutoff = cutoff_freq / nyquist
        self.b, self.a = butter(N=2, Wn=normalized_cutoff, btype='low', analog=False)
        self.x_history = np.zeros(3)
        self.y_history = np.zeros(3)

    def __call__(self, array):
        return lfilter(self.b, self.a, array)

    def process_scalar(self, newValue):
        # Shift history
        self.x_history[2] = self.x_history[1]
        self.x_history[1] = self.x_history[0]
        self.x_history[0] = newValue

        self.y_history[2] = self.y_history[1]
        self.y_history[1] = self.y_history[0]

        # Calculate the output using the filter difference equation
        self.y_history[0] = (self.b[0] * self.x_history[0] +
                             self.b[1] * self.x_history[1] +
                             self.b[2] * self.x_history[2] -
                             self.a[1] * self.y_history[1] -
                             self.a[2] * self.y_history[2])

        return self.y_history[0]

class Butter2_manual:
    def __init__(self, cutoff_freq, sample_rate=1.0):
        self.a, self.b = self._calculate_coefficients(cutoff_freq, sample_rate)
        self.x_history = np.zeros(3)
        self.y_history = np.zeros(3)

    def _calculate_coefficients(self, cutoff_freq, sample_rate):
        # Angular frequency and pre-warping for bilinear transform
        omega = np.tan(np.pi * cutoff_freq / sample_rate)
        omega_squared = omega ** 2
        sqrt2 = np.sqrt(2.0)
        denom = 1.0 + sqrt2 * omega + omega_squared

        # Feedforward (numerator) coefficients
        b = np.array([omega_squared / denom,
                      2.0 * omega_squared / denom,
                      omega_squared / denom])

        # Feedback (denominator) coefficients, with a[0] normalized to 1
        a = np.array([1.0,
                      2.0 * (omega_squared - 1.0) / denom,
                      (1.0 - sqrt2 * omega + omega_squared) / denom])

        return a, b

    def __call__(self, array):
        result = np.zeros_like(array)
        for i in range(len(array)):
            result[i] = self.process_scalar(array[i])
        return result

    def process_scalar(self, newValue):
        # Shift history
        self.x_history[2] = self.x_history[1]
        self.x_history[1] = self.x_history[0]
        self.x_history[0] = newValue

        self.y_history[2] = self.y_history[1]
        self.y_history[1] = self.y_history[0]

        # Calculate the output using the filter difference equation
        self.y_history[0] = (self.b[0] * self.x_history[0] +
                             self.b[1] * self.x_history[1] +
                             self.b[2] * self.x_history[2] -
                             self.a[1] * self.y_history[1] -
                             self.a[2] * self.y_history[2])

        return self.y_history[0]
