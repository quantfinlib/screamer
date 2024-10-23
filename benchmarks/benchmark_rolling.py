import os
import timeit
import numpy as np
import pandas as pd

from  screamer import SMA, RollingStd, RollingSkew, RollingKurt

# Rolling Mean
def pandas_rolling_mean(array, window_size):
    return pd.Series(array).rolling(window=window_size).mean()

# Rolling Standard Deviation
def pandas_rolling_std(array, window_size):
    return pd.Series(array).rolling(window=window_size).std()

# Rolling Skewness
def pandas_rolling_skew(array, window_size):
    return pd.Series(array).rolling(window=window_size).skew()

# Rolling Kurtosis
def pandas_rolling_kurt(array, window_size):
    return pd.Series(array).rolling(window=window_size).kurt()

# Time function calls
def time_function(func, *args):
    start_time = timeit.default_timer()
    result = func(*args)
    end_time = timeit.default_timer()
    return end_time - start_time


experiments = []

for p in np.arange(2, 9): # power 10^p: 100 ... 100_000_000
    for m in np.arange(1,10): # multiplier
        N = m * 10**p

        # Upper bound for vector length
        if N > 100_000_000:
            continue

        print('testing size', N)

        x = np.random.normal(size=N)

        for window_size in [10, 100, 1000, 10_000, 100_000, 1_000_000]:

            if window_size > N - 3:
                continue

            experiments.append({
                'lib': 'pandas',
                'func': 'RollingMean',
                'N': N,
                'window_size': window_size,
                'time': time_function(pandas_rolling_mean, x, window_size)
            })

            indicator = SMA(window_size)
            experiments.append({
                'lib': 'screamer',
                'func': 'RollingMean',
                'N': N,
                'window_size': window_size,
                'time': time_function(indicator.transform, x)
            })

            experiments.append({
                'lib': 'pandas',
                'func': 'RollingStd',
                'N': N,
                'window_size': window_size,
                'time': time_function(pandas_rolling_std, x, window_size)
            })

            indicator = RollingStd(window_size)
            experiments.append({
                'lib': 'screamer',
                'func': 'RollingStd',
                'N': N,
                'window_size': window_size,
                'time': time_function(indicator.transform, x)
            })            


            experiments.append({
                'lib': 'pandas',
                'func': 'RollingSkew',
                'N': N,
                'window_size': window_size,
                'time': time_function(pandas_rolling_skew, x, window_size)
            })

            indicator = RollingSkew(window_size)
            experiments.append({
                'lib': 'screamer',
                'func': 'RollingSkew',
                'N': N,
                'window_size': window_size,
                'time': time_function(indicator.transform, x)
            })            

            experiments.append({
                'lib': 'pandas',
                'func': 'RollingKurt',
                'N': N,
                'window_size': window_size,
                'time': time_function(pandas_rolling_kurt, x, window_size)
            })

            indicator = RollingKurt(window_size)
            experiments.append({
                'lib': 'screamer',
                'func': 'RollingKurt',
                'N': N,
                'window_size': window_size,
                'time': time_function(indicator.transform, x)
            })   


# Convert excperiments to a Pandas DataFrame
df = pd.DataFrame(experiments)


# Get the directory of the current script
script_dir = os.path.dirname(os.path.abspath(__file__))

# Define the path to save the plot in the same directory as the script
save_path = os.path.join(script_dir, 'benchmark_rolling.csv')

# Save to disk
df.to_csv(save_path, index=False)

