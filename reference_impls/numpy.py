import numpy as np

def numpy__rolling_mean__cumsum(array, window_size):
    ans = np.cumsum(array)
    ans[window_size:] = ans[window_size:] - ans[:-window_size]
    return ans / window_size

def numpy__rolling_mean__stride(array, window_size):
    windowed_array = np.lib.stride_tricks.sliding_window_view(array, window_size)
    ans  = np.mean(windowed_array, axis=-1)
    return np.concatenate((np.full(window_size-1, np.nan), ans))

def numpy__rolling_std__stride(array, window_size):
    windowed_array = np.lib.stride_tricks.sliding_window_view(array, window_size)
    ans = np.std(windowed_array, axis=-1, ddof=1)
    return np.concatenate((np.full(window_size-1, np.nan), ans))

def numpy__rolling_skew__stride(array, window_size):
    windowed_array = np.lib.stride_tricks.sliding_window_view(array, window_size)
    mean = np.mean(windowed_array, axis=-1)
    std = np.std(windowed_array, axis=-1, ddof=1)
    skewness = np.mean(((windowed_array - mean[:, None]) / std[:, None])**3, axis=-1)
    skewness = skewness * window_size * window_size / (window_size-1) / (window_size-2)
    return np.concatenate((np.full(window_size-1, np.nan), skewness))

def numpy__rolling_kurt__stride(array, window_size):
    windowed_array = np.lib.stride_tricks.sliding_window_view(array, window_size)
    mean = np.mean(windowed_array, axis=-1)
    std = np.std(windowed_array, axis=-1, ddof=1)
    kurtosis = np.mean(((windowed_array - mean[:, None]) / std[:, None])**4, axis=-1) - 3
    return np.concatenate((np.full(window_size-1, np.nan), kurtosis))


import numpy as np

def numpy__rolling_kurt__stride(array, window_size):
    N = window_size
    x = np.lib.stride_tricks.sliding_window_view(array, N)

    mean = np.mean(x, axis=1)
    std = np.std(x, axis=1, ddof=1)
    sum_xx = np.sum(x**2, axis=1)
    sum_xxx = np.sum(x**3, axis=1)
    sum_xxxx = np.sum(x**4, axis=1)

    # Compute m4 (fourth central moment)
    m4 = sum_xxxx - 4 * mean * sum_xxx + 6 * mean * mean * sum_xx - 3 * N * mean**4

    # Calculate numerator and denominator for kurtosis
    numerator = N * (N + 1) * m4
    denominator = (N - 1) * (N - 2) * (N - 3) * std**4

    # Calculate adjustment term
    adjustment = (3 * (N - 1)**2) / ((N - 2)*(N - 3))

    # Compute kurtosis
    kurtosis = numerator / denominator - adjustment
    return np.concatenate((np.full(window_size-1, np.nan), kurtosis))


def numpy__rolling_min__stride(array, window_size):
    windowed_array = np.lib.stride_tricks.sliding_window_view(array, window_size)
    ans = np.min(windowed_array, axis=-1)
    return np.concatenate((np.full(window_size-1, np.nan), ans))

def numpy__rolling_max__stride(array, window_size):
    windowed_array = np.lib.stride_tricks.sliding_window_view(array, window_size)
    ans = np.max(windowed_array, axis=-1)
    return np.concatenate((np.full(window_size-1, np.nan), ans))

def numpy__rolling_median__stride(array, window_size):
    windowed_array = np.lib.stride_tricks.sliding_window_view(array, window_size)
    ans = np.median(windowed_array, axis=-1)
    return np.concatenate((np.full(window_size-1, np.nan), ans))

def numpy__rolling_quantile__stride(array, window_size, quantile=0.75):
    windowed_array = np.lib.stride_tricks.sliding_window_view(array, window_size)
    ans = np.percentile(windowed_array, quantile*100, axis=-1)
    return np.concatenate((np.full(window_size-1, np.nan), ans))

def numpy__diff(array, window_size):
    ans = np.empty_like(array)
    ans[window_size:] = array[window_size:] - array[:-window_size]
    return ans

def numpy__lag(array, window_size):
    ans = np.empty_like(array)
    ans[window_size:] = array[:-window_size]
    return ans