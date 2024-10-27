import screamer as sc

def RollingSum__screamer(array, window_size):
    return sc.RollingSum(window_size).transform(array)

def RollingMean__screamer(array, window_size):
    return sc.RollingMean(window_size).transform(array)

def RollingMean2__screamer(array, window_size):
    return sc.RollingMean2(window_size)(array)

def RollingVar__screamer(array, window_size):
    return sc.RollingVar(window_size).transform(array)

def RollingStd__screamer(array, window_size):
    return sc.RollingStd(window_size).transform(array)

def RollingSkew__screamer(array, window_size):
    return sc.RollingSkew(window_size).transform(array)

def RollingKurt__screamer(array, window_size):
    return sc.RollingKurt(window_size).transform(array)

def RollingMin__screamer(array, window_size):
    return sc.RollingMin(window_size).transform(array)

def RollingMax__screamer(array, window_size):
    return sc.RollingMax(window_size).transform(array)

def RollingMedian__screamer(array, window_size):
    return sc.RollingMedian(window_size).transform(array)

"""
def RollingQuantile__screamer(array, window_size, quantile):
    return sc.RollingQuantile(window_size, quantile).transform(array)
"""

def Diff__screamer(array, window_size):
    return sc.Diff(window_size).transform(array)

def Lag__screamer(array, window_size):
    return sc.Lag(window_size).transform(array)
