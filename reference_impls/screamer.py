import screamer as sc

"""
def RollingQuantile__screamer(array, window_size, quantile):
    return sc.RollingQuantile(window_size, quantile).transform(array)
"""

def Diff__screamer(array, window_size):
    return sc.Diff(window_size)(array)

def Lag__screamer(array, window_size):
    return sc.Lag(window_size)(array)

def RollingMean__screamer(array, window_size):
    return sc.RollingMean(window_size)(array)

def RollingStd__screamer(array, window_size):
    return sc.RollingStd(window_size)(array)

def RollingVar__screamer(array, window_size):
    return sc.RollingVar(window_size)(array)

def RollingZscore__screamer(array, window_size):
    return sc.RollingZscore(window_size)(array)

def RollingKurt__screamer(array, window_size):
    return sc.RollingKurt(window_size)(array)

def Ffill__screamer(array):
    return sc.Ffill()(array)

def FillNa__screamer(array, fill):
    return sc.FillNa(fill)(array)

def RollingMin__screamer(array, window_size):
    return sc.RollingMin(window_size)(array)

def RollingMax__screamer(array, window_size):
    return sc.RollingMax(window_size)(array)

def RollingSum__screamer(array, window_size):
    return sc.RollingSum(window_size)(array)

def RollingSkew__screamer(array, window_size):
    return sc.RollingSkew(window_size)(array)

def RollingMedian__screamer(array, window_size):
    return sc.RollingMedian(window_size)(array)

def RollingQuantile__screamer(array, window_size, quantile=0.75):
    return sc.RollingQuantile(window_size, quantile)(array)

def Return__screamer(array, window_size):
    return sc.Return(window_size)(array)

def LogReturn__screamer(array, window_size):
    return sc.LogReturn(window_size)(array)

def EwMean__screamer(array, window_size):
    alpha = 1 / (1 + window_size)
    return sc.EwMean(alpha=alpha)(array)
