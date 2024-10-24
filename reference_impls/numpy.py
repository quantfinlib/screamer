import numpy as np

def numpy__rolling_mean__cumsum(array, window_size):
    ans = np.cumsum(array)
    ans[window_size:] -= ans[:window_size]
    return ans / window_size

def numpy__rolling_mean__stride(array, window_size):
    windowed_array = np.lib.stride_tricks.sliding_window_view(array, window_size)
    return np.mean(windowed_array, axis=-1)

def numpy__rolling_std__stride(array, window_size):
    windowed_array = np.lib.stride_tricks.sliding_window_view(array, window_size)
    return np.std(windowed_array, axis=-1, ddof=1)

def numpy__rolling_skew__stride(array, window_size):
    windowed_array = np.lib.stride_tricks.sliding_window_view(array, window_size)
    mean = np.mean(windowed_array, axis=-1)
    std = np.std(windowed_array, axis=-1, ddof=1)
    skewness = np.mean(((windowed_array - mean[:, None]) / std[:, None])**3, axis=-1)
    return skewness

def numpy__rolling_kurt__stride(array, window_size):
    windowed_array = np.lib.stride_tricks.sliding_window_view(array, window_size)
    mean = np.mean(windowed_array, axis=-1)
    std = np.std(windowed_array, axis=-1, ddof=1)
    kurtosis = np.mean(((windowed_array - mean[:, None]) / std[:, None])**4, axis=-1) - 3
    return kurtosis

def numpy__rolling_min__stride(array, window_size):
    windowed_array = np.lib.stride_tricks.sliding_window_view(array, window_size)
    return np.min(windowed_array, axis=-1)

def numpy__rolling_max__stride(array, window_size):
    windowed_array = np.lib.stride_tricks.sliding_window_view(array, window_size)
    return np.max(windowed_array, axis=-1)

def numpy__rolling_median__stride(array, window_size):
    windowed_array = np.lib.stride_tricks.sliding_window_view(array, window_size)
    return np.median(windowed_array, axis=-1)

def numpy__rolling_quantile__stride(array, window_size, q=0.75):
    windowed_array = np.lib.stride_tricks.sliding_window_view(array, window_size)
    return np.percentile(windowed_array, q*100, axis=-1)
