import pandas as pd

def pandas__rolling_mean(array, window_size):
    return pd.Series(array).rolling(window=window_size).mean()

def pandas__rolling_std(array, window_size):
    return pd.Series(array).rolling(window=window_size).std()

def pandas__rolling_skew(array, window_size):
    return pd.Series(array).rolling(window=window_size).skew()

def pandas__rolling_kurt(array, window_size):
    return pd.Series(array).rolling(window=window_size).kurt()

def pandas__rolling_min(array, window_size):
    return pd.Series(array).rolling(window=window_size).min()

def pandas__rolling_max(array, window_size):
    return pd.Series(array).rolling(window=window_size).max()

def pandas__rolling_median(array, window_size):
    return pd.Series(array).rolling(window=window_size).median()

def pandas__rolling_quantile(array, window_size, q=0.75):
    return pd.Series(array).rolling(window=window_size).quantile(q)
