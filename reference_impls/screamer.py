import screamer as sc

def screamer__rolling_mean(array, window_size):
    obj = sc.RollingMean(window_size)
    return obj.transform(array)
