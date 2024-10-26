import os
import numpy as np
import pandas as pd
import reference_impls
import timeit


def time_call(callable_name, args, **kwargs):

    function_call = f'reference_impls.{callable_name}({args})'

    start_time = timeit.default_timer()
    result = eval(function_call, {'reference_impls': reference_impls}, kwargs)
    end_time = timeit.default_timer()

    return end_time - start_time


array_sizes = [f*10**p for p in range(1, 6) for f in [1,2,5]] + [10**6]
window_sizes = [10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000]

extra_args = {
    'quantile': 0.75
}

results = []
for repeats in range(5):
    for _, row in reference_impls.all().iterrows():
        row = row.to_dict()
        if row['func'] != 'rolling_mean2':
            continue
        for n in array_sizes:

            args = extra_args.copy()
            args['array'] = np.random.normal(size=n)
            
            for window_size in window_sizes:

                if window_size >= n:
                    break

                if row['lib'] == 'numpy':
                    if window_size > 1000:
                        break

                args['window_size'] = window_size

                row['n'] = n
                row['window_size'] = window_size
                row['time'] = time_call(row['callable'], row['args'], **args)

                results.append(row.copy())
                print(row)

results = pd.DataFrame(results)
print(results)


# Get the directory of the current script
script_dir = os.path.dirname(os.path.abspath(__file__))

# Define the path to save the plot in the same directory as the script
save_path = os.path.join(script_dir, 'experiments', 'benchmark_results.csv')

# Save to disk
results.to_csv(save_path, index=False)

