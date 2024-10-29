import numpy as np

def RollingSum__numpy__cumsum(array, window_size):
    ans = np.cumsum(array)
    ans[window_size:] = ans[window_size:] - ans[:-window_size]
    return ans 

def RollingSum__numpy__stride(array, window_size):
    windowed_array = np.lib.stride_tricks.sliding_window_view(array, window_size)
    ans = np.sum(windowed_array, axis=-1)
    return np.concatenate((np.full(window_size-1, np.nan), ans))

def RollingMean__numpy__cumsum(array, window_size):
    ans = np.cumsum(array)
    ans[window_size:] = ans[window_size:] - ans[:-window_size]
    return ans / window_size

def RollingMean__numpy__stride(array, window_size):
    windowed_array = np.lib.stride_tricks.sliding_window_view(array, window_size)
    ans = np.mean(windowed_array, axis=-1)
    return np.concatenate((np.full(window_size-1, np.nan), ans))

def RollingStd__numpy__stride(array, window_size):
    windowed_array = np.lib.stride_tricks.sliding_window_view(array, window_size)
    ans = np.std(windowed_array, axis=-1, ddof=1)
    return np.concatenate((np.full(window_size-1, np.nan), ans))

def RollingVar__numpy__stride(array, window_size):
    windowed_array = np.lib.stride_tricks.sliding_window_view(array, window_size)
    ans = np.var(windowed_array, axis=-1, ddof=1)
    return np.concatenate((np.full(window_size-1, np.nan), ans))

def RollingZscore__numpy__stride(array, window_size):
    windowed_array = np.lib.stride_tricks.sliding_window_view(array, window_size)
    mean = np.mean(windowed_array, axis=-1)
    std = np.std(windowed_array, axis=-1, ddof=1)
    ans = (windowed_array[:,-1] - mean) / std
    return np.concatenate((np.full(window_size-1, np.nan), ans))

def RollingSkew__numpy__stride(array, window_size):
    windowed_array = np.lib.stride_tricks.sliding_window_view(array, window_size)
    mean = np.mean(windowed_array, axis=-1)
    std = np.std(windowed_array, axis=-1, ddof=1)
    skewness = np.mean(((windowed_array - mean[:, None]) / std[:, None])**3, axis=-1)
    skewness = skewness * window_size * window_size / (window_size-1) / (window_size-2)
    return np.concatenate((np.full(window_size-1, np.nan), skewness))

def RollingKurt__numpy__stride(array, window_size):
    N = window_size
    x = np.lib.stride_tricks.sliding_window_view(array, N)

    mean = np.mean(x, axis=1)
    std = np.std(x, axis=1, ddof=1)
    sum_xx = np.sum(x**2, axis=1)
    sum_xxx = np.sum(x**3, axis=1)
    sum_xxxx = np.sum(x**4, axis=1)

    m4 = sum_xxxx - 4 * mean * sum_xxx + 6 * mean * mean * sum_xx - 3 * N * mean**4
    numerator = N * (N + 1) * m4
    denominator = (N - 1) * (N - 2) * (N - 3) * std**4
    adjustment = (3 * (N - 1)**2) / ((N - 2)*(N - 3))
    kurtosis = numerator / denominator - adjustment
    return np.concatenate((np.full(window_size-1, np.nan), kurtosis))

def RollingMin__numpy__stride(array, window_size):
    windowed_array = np.lib.stride_tricks.sliding_window_view(array, window_size)
    ans = np.min(windowed_array, axis=-1)
    return np.concatenate((np.full(window_size-1, np.nan), ans))

def RollingMax__numpy__stride(array, window_size):
    windowed_array = np.lib.stride_tricks.sliding_window_view(array, window_size)
    ans = np.max(windowed_array, axis=-1)
    return np.concatenate((np.full(window_size-1, np.nan), ans))

def RollingMedian__numpy__stride(array, window_size):
    windowed_array = np.lib.stride_tricks.sliding_window_view(array, window_size)
    ans = np.median(windowed_array, axis=-1)
    return np.concatenate((np.full(window_size-1, np.nan), ans))

def RollingQuantile__numpy__stride(array, window_size, quantile=0.75):
    windowed_array = np.lib.stride_tricks.sliding_window_view(array, window_size)
    ans = np.percentile(windowed_array, quantile*100, axis=-1)
    return np.concatenate((np.full(window_size-1, np.nan), ans))

def Diff__numpy(array, window_size):
    ans = np.empty_like(array)
    ans[:window_size] = 0.0
    ans[window_size:] = array[window_size:] - array[:-window_size]
    return ans

def Lag__numpy(array, window_size):
    ans = np.empty_like(array)
    ans[window_size:] = array[:-window_size]
    return ans

def FillNa__numpy(array, fill):
    ans = array.copy()
    ans[np.isnan(ans)] = fill
    return ans

def _np_ffill(arr, axis):
    idx_shape = tuple([slice(None)] + [np.newaxis] * (len(arr.shape) - axis - 1))
    idx = np.where(~np.isnan(arr), np.arange(arr.shape[axis])[idx_shape], 0)
    np.maximum.accumulate(idx, axis=axis, out=idx)
    slc = [np.arange(k)[tuple([slice(None) if dim==i else np.newaxis
        for dim in range(len(arr.shape))])]
        for i, k in enumerate(arr.shape)]
    slc[axis] = idx
    return arr[tuple(slc)]

def Ffill__numpy(array):
    return _np_ffill(array, 0)

def Return__numpy(array, window_size):
    ans = np.empty_like(array)
    ans[:window_size] = np.nan
    ans[window_size:] = (array[window_size:] - array[:-window_size]) / array[:-window_size]
    return ans

def LogReturn__numpy(array, window_size):
    ans = np.empty_like(array)
    ans[:window_size] = np.nan
    ans[window_size:] = np.log(array[window_size:] / array[:-window_size])
    return ans

"""
todo
def EwSkew__numpy(array, window_size):
def EwKurt_numpy(array, window_size):
"""    

def Clip__numpy(array, lower=-0.1, upper=0.5):
    return np.clip(array, a_min=lower, a_max=upper)

def RollingRms__numpy(array, window_size):
    ans = RollingMean__numpy__cumsum(array**2, window_size)
    return np.sqrt(ans)

def Abs__numpy(array):
    return np.abs(array)

def Log__numpy(array):
    return np.log(array)

def Exp__numpy(array):
    return np.exp(array)

def Sqrt__numpy(array):
    return np.sqrt(array)

def Sign__numpy(array):
    return np.sign(array)

