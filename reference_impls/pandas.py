import pandas as pd
import numpy as np

def RollingSum__pandas(array, window_size):
    return pd.Series(array).rolling(window=window_size).sum().to_numpy()

def RollingMean__pandas(array, window_size):
    return pd.Series(array).rolling(window=window_size).mean().to_numpy()

def RollingStd__pandas(array, window_size):
    return pd.Series(array).rolling(window=window_size).std().to_numpy()

def RollingVar__pandas(array, window_size):
    return pd.Series(array).rolling(window=window_size).var().to_numpy()

def RollingZscore__pandas(array, window_size):
    r = pd.Series(array).rolling(window=window_size)
    m = r.mean().to_numpy()
    s = r.std(ddof=1).to_numpy()
    return (array - m) / s

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

def Ffill__pandas(array):
    return pd.Series(array).ffill().to_numpy()

def FillNa__pandas(array, fill):
    return pd.Series(array).fillna(value=fill).to_numpy()

def Return__pandas(array, window_size):
    return pd.Series(array).pct_change(periods=window_size).to_numpy()

def LogReturn__pandas(array, window_size):
    return np.log1p(Return__pandas(array, window_size))
