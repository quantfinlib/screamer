import pandas as pd

def pandas__rolling_sum(array, window_size):
    return pd.Series(array).rolling(window=window_size).sum().to_numpy()

def pandas__rolling_mean(array, window_size):
    return pd.Series(array).rolling(window=window_size).mean().to_numpy()

def pandas__rolling_std(array, window_size):
    return pd.Series(array).rolling(window=window_size).std().to_numpy()

def pandas__rolling_skew(array, window_size):
    return pd.Series(array).rolling(window=window_size).skew().to_numpy()

def pandas__rolling_kurt(array, window_size):
    return pd.Series(array).rolling(window=window_size).kurt().to_numpy()

def pandas__rolling_min(array, window_size):
    return pd.Series(array).rolling(window=window_size).min().to_numpy()

def pandas__rolling_max(array, window_size):
    return pd.Series(array).rolling(window=window_size).max().to_numpy()

def pandas__rolling_median(array, window_size):
    return pd.Series(array).rolling(window=window_size).median().to_numpy()

def pandas__rolling_quantile(array, window_size, quantile=0.75):
    return pd.Series(array).rolling(window=window_size).quantile(quantile).to_numpy()
