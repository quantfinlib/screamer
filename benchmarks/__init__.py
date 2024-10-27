import os
import glob
import pandas as pd

def read_expiriments(func=None):
    script_dir = os.path.dirname(os.path.abspath(__file__))

    if func:
        file_name_pattern = os.path.join(script_dir, 'experiments', f'bm__{func}__*.csv')
    else:
        file_name_pattern = os.path.join(script_dir, 'experiments', f'bm__*.csv')
    experiments = []
    for filename in glob.glob(file_name_pattern):
        df = pd.read_csv(
            filename,
            dtype={
                'func': str, 
                'lib': str, 
                'var': str,
                'window_size': int, 
                'n': int, 
                'time': float
            },
            keep_default_na=False
        )
        experiments.append(df)
    experiments = pd.concat(experiments,axis=0)

    # Group by all columns except 'time' and reduce to the minimum 'time' for each group
    experiments = experiments.groupby(['func', 'lib', 'var', 'window_size', 'n'], as_index=False).agg({'time': 'median'})

    # sort 
    experiments = experiments.sort_values(by=['func', 'lib', 'var', 'window_size', 'n'])

    return experiments
