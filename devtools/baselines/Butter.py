import numpy as np
from scipy.signal import lfilter, butter

class Butter_scipy:
    def __init__(self, order, cutoff_freq):
        # Generate Butterworth filter coefficients
        self.b, self.a = butter(order, cutoff_freq, btype='low', analog=False)

    def __call__(self, array):
        # Apply the filter to the array using lfilter
        return lfilter(self.b, self.a, array)
