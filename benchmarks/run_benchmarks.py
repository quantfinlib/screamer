import os
import numpy as np
import pandas as pd
import reference_impls
import timeit
import argparse


def time_call(callable_name, args, **kwargs):

    function_call = f'reference_impls.{callable_name}({args})'

    start_time = timeit.default_timer()
    result = eval(function_call, {'reference_impls': reference_impls}, kwargs)
    end_time = timeit.default_timer()

    return end_time - start_time


array_sizes = [int(f*10**p) for p in range(1, 6) for f in [1, 1.6, 2.5, 4, 6.3]] + [10**6]
window_sizes = [10, 100,  1000, 10000]

extra_args = {
    'quantile': 0.75,
    'fill': 0.0,
    'alpha': 0.1
}

def main():
    # Initialize the parser
    parser = argparse.ArgumentParser(description="Run benchmarks.")

    # Add a positional argument named `func`, with a default value
    parser.add_argument("--func", type=str, default=None, help="Function name filter")
    parser.add_argument("--lib", type=str, default=None, help="lib name filter")
    parser.add_argument("--repeat", type=int, default=11, help="number of repeats")

    # Parse the arguments
    cmd_args = parser.parse_args()
  
    # Geta list of function
    functions = reference_impls.all()
    if cmd_args.func:
        functions = functions[functions['func'] == cmd_args.func]
    if cmd_args.lib:
        functions = functions[functions['lib'] == cmd_args.lib]
    

    # Loop all function
    for _, row in functions.iterrows():
        
        print(row['callable'] + ' ', end="")
        results = []
        for repeats in range(cmd_args.repeat):
            print('.', end='', flush=True)
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
        
        results = pd.DataFrame(results)
        # Get the directory of the current script
        script_dir = os.path.dirname(os.path.abspath(__file__))

        # Define the path to save the plot in the same directory as the script
        save_path = os.path.join(script_dir, 'experiments', f"bm__{row['callable']}.csv")

        # Save to disk
        results.to_csv(save_path, index=False)
        print(' done.')



# Entry point for the script
if __name__ == "__main__":
    main()
