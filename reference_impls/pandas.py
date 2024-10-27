import pandas as pd

def RollingSum__pandas(array, window_size):
    return pd.Series(array).rolling(window=window_size).sum().to_numpy()

def RollingMean__pandas(array, window_size):
    return pd.Series(array).rolling(window=window_size).mean().to_numpy()

def RollingMean2__pandas(array, window_size):
    return pd.Series(array).rolling(window=window_size).mean().to_numpy()

def RollingStd__pandas(array, window_size):
    return pd.Series(array).rolling(window=window_size).std().to_numpy()

def RollingSkew__pandas(array, window_size):
    return pd.Series(array).rolling(window=window_size).skew().to_numpy()

def RollingKurt__pandas(array, window_size):
    return pd.Series(array).rolling(window=window_size).kurt().to_numpy()

def RollingMin__pandas(array, window_size):
    return pd.Series(array).rolling(window=window_size).min().to_numpy()

def RollingMax__pandas(array, window_size):
    return pd.Series(array).rolling(window=window_size).max().to_numpy()

def RollingMedian__pandas(array, window_size):
    return pd.Series(array).rolling(window=window_size).median().to_numpy()

def RollingQuantile__pandas(array, window_size, quantile=0.75):
    return pd.Series(array).rolling(window=window_size).quantile(quantile).to_numpy()

def Diff__pandas(array, window_size):
    return pd.Series(array).diff(periods=window_size).to_numpy()

def Lag__pandas(array, window_size):
    return pd.Series(array).shift(window_size).to_numpy()
