import numpy as np

def _rms(x):
    return np.sqrt(np.mean(x**2))

class RollingOU_numpy:
    def __init__(self, window_size, output=0):
        self.window_size = window_size
        self.output = output

    def __call__(self, array):
        if len(array) < self.window_size:
            raise ValueError("Input array length must be at least the window size")

        # Stride over the array to create rolling windows
        windows = np.lib.stride_tricks.sliding_window_view(array, self.window_size)
        num_rows = windows.shape[0]
        x = windows[:, :-1]
        y = windows[:, 1:]

        a = np.zeros(shape=array.shape)
        b = np.zeros(shape=array.shape)
        e = np.zeros(shape=array.shape)
        a[:self.window_size] = np.nan
        b[:self.window_size] = np.nan
        e[:self.window_size] = np.nan
         
        for i in range(num_rows):
            k = i + self.window_size - 1
            a[k], b[k] = np.polyfit(x[i,:], y[i,:], 1)
            e[k] = _rms(y[i,:] - a[i]*x[i,:] - b[i])
        
        # Continuous time version with dt = 1
        # mrr = -np.log(a)
        # mu = b / (1.0 - a)
        # sigma = e * np.sqrt(-2 * mrr / (1 - a*a))

        # Discrete OU
        mrr = 1.0 - a
        mu = b / mrr

        if self.output == 0:
            return mrr
        if self.output == 1:
            return mu
        if self.output == 2:
            return mu - y[:,-1]
        if self.output == 3:
            return e

