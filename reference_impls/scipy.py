import scipy.special
from scipy.signal import butter, lfilter

def Erf__scipy(array):
    return scipy.special.erf(array)

def Erfc__scipy(array):
    return scipy.special.erfc(array)


def _Butter(array, cutoff_freq, order):
    # Normalized cutoff frequency (Nyquist frequency = 0.5 in this case)
    nyquist_freq = 0.5  # Since sampleRate = 1, Nyquist frequency is 0.5
    normalized_cutoff = cutoff_freq / nyquist_freq

    # Generate Butterworth filter coefficients for the given cutoff and order
    b, a = butter(order, normalized_cutoff, btype='low', analog=False)
    return lfilter(b, a, array)

def Butter2__scipy(array, cutoff_freq):
    return _Butter(array, cutoff_freq, order=2)

