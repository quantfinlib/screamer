import screamer as sc

def Diff__screamer(array, window_size):
    return sc.Diff(window_size)(array)

def Lag__screamer(array, window_size):
    return sc.Lag(window_size)(array)

def Ffill__screamer(array):
    return sc.Ffill()(array)

def FillNa__screamer(array, fill):
    return sc.FillNa(fill)(array)

def Return__screamer(array, window_size):
    return sc.Return(window_size)(array)

def LogReturn__screamer(array, window_size):
    return sc.LogReturn(window_size)(array)

def Clip__screamer(array, lower=-0.1, upper=0.5):
    return sc.Clip(lower=lower,upper=upper)(array)




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

def RollingRms__screamer(array, window_size):
    return sc.RollingRms(window_size)(array)




def EwMean__screamer(array, window_size):
    alpha = 1 / (1 + window_size)
    return sc.EwMean(alpha=alpha)(array)

def EwVar__screamer(array, window_size):
    alpha = 1 / (1 + window_size)
    return sc.EwVar(alpha=alpha)(array)

def EwStd__screamer(array, window_size):
    alpha = 1 / (1 + window_size)
    return sc.EwStd(alpha=alpha)(array)

def EwZscore__screamer(array, window_size):
    alpha = 1 / (1 + window_size)
    return sc.EwZscore(alpha=alpha)(array)

def EwSkew__screamer(array, window_size):
    alpha = 1 / (1 + window_size)
    return sc.EwSkew(alpha=alpha)(array)

def EwKurt__screamer(array, window_size):
    alpha = 1 / (1 + window_size)
    return sc.EwKurt(alpha=alpha)(array)

def EwRms__screamer(array, window_size):
    alpha = 1 / (1 + window_size)
    return sc.EwRms(alpha=alpha)(array)

def Abs__screamer(array):
    return sc.Abs()(array)

def Exp__screamer(array):
    return sc.Exp()(array)

def Log__screamer(array):
    return sc.Log()(array)

def Sqrt__screamer(array):
    return sc.Sqrt()(array)

def Sign__screamer(array):
    return sc.Sign()(array)

def Erf__screamer(array):
    return sc.Erf()(array)

def Erfc__screamer(array):
    return sc.Erfc()(array)

def Butter2__screamer(array, cutoff_freq):
    return sc.Butter2(cutoff_freq)(array)

def RollingPoly1__screamer(array, window_size, derivative_order):
    return sc.RollingPoly1(window_size, derivative_order)(array)

def RollingPoly2__screamer(array, window_size, derivative_order):
    return sc.RollingPoly2(window_size, derivative_order)(array)
