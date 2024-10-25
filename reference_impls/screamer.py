import screamer as sc

def screamer__rolling_sum(array, window_size):
    return sc.RollingSum(window_size).transform(array)

def screamer__rolling_mean(array, window_size):
    return sc.RollingMean(window_size).transform(array)

def screamer__rolling_var(array, window_size):
    return sc.RollingVar(window_size).transform(array)

def screamer__rolling_std(array, window_size):
    return sc.RollingStd(window_size).transform(array)

def screamer__rolling_skew(array, window_size):
    return sc.RollingSkew(window_size).transform(array)

def screamer__rolling_kurt(array, window_size):
    return sc.RollingKurt(window_size).transform(array)

def screamer__rolling_min(array, window_size):
    return sc.RollingMin(window_size).transform(array)

def screamer__rolling_max(array, window_size):
    return sc.RollingMax(window_size).transform(array)

def screamer__rolling_median(array, window_size):
    return sc.RollingMedian(window_size).transform(array)

"""
def screamer__rolling_quantile(array, window_size, quantile):
    return sc.RollingQuantile(window_size, quantile).transform(array)
"""

def screamer__diff(array, window_size):
    return sc.Diff(window_size).transform(array)

def screamer__lag(array, window_size):
    return sc.Lag(window_size).transform(array)