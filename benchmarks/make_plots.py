import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# Get the directory of the current script
script_dir = os.path.dirname(os.path.abspath(__file__))

# Read the data
df = pd.read_csv(
    os.path.join(script_dir, 'experiments', 'benchmark_rolling.csv'),
    dtype={'lib': str, 'func': str, 'N': int, 'window_size': int, 'time': float}
)


def make_func_plot(df_filtered, func_name):
    # Sort by window_size and assign colors
    sorted_window_sizes = sorted(df_filtered['window_size'].unique())
    colors = {ws: f'C{i}' for i, ws in enumerate(sorted_window_sizes)}

    # Plot
    plt.figure(figsize=(8,6))
    for (lib, window_size), group in df_filtered.groupby(['lib', 'window_size']):
        linestyle = '-' if lib == 'pandas' else '-'
        markerstyle = None if lib == 'pandas' else 'o'
        plt.plot(np.log10(group['N']), np.log10(group['time']),
                label=f'{lib}, window_size={window_size:>10,.0f}'.replace(',','.'), 
                color=colors[window_size], 
                linestyle=linestyle,
                marker=markerstyle
            )
    plt.xlabel('log10(N)')
    plt.ylabel('log10(time)')
    plt.xlim(3,8)
    plt.title(f'Log-Log Plot of N vs Time for {func_name}')
    plt.legend(loc='best')
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(os.path.join(script_dir, 'plots', f'benchmark_{func_name.lower()}.png'))


    # Create a pivot table to calculate the ratio of pandas time to screamer time for each N and window_size
    pivot = df_filtered.pivot_table(index=['N', 'window_size'], columns='lib', values='time')

    # Calculate the ratio of pandas time to screamer time
    pivot['ratio'] = pivot['pandas'] / pivot['screamer']

    # Plot
    plt.figure(figsize=(8,6))
    for window_size, group in pivot.groupby('window_size'):
        plt.semilogx(group.index.get_level_values('N'), group['ratio'], 
                label=f'{window_size:>10,.0f}'.replace(',', '.'), color=colors[window_size])

    # Calculate the average of the last values
    last_values = []
    for window_size, group in pivot.groupby('window_size'):
        last_values.append(group['ratio'].iloc[-1])  # Get the last value of each series
    average_last_value = sum(last_values) / len(last_values)
    plt.axhline(y=average_last_value, color='red', linestyle='--', label=f'Average {average_last_value:.1f}')

    plt.xlabel('Series length N')
    plt.ylabel('Speedup factor pandas / screamer')
    plt.ylim(bottom=0)
    plt.title(f'{func_name} speedup factor pandas/screamer')
    plt.legend(title='window_size', loc='best')
    plt.grid(True)
    plt.tight_layout()


    plt.savefig(os.path.join(script_dir, 'plots', f'benchmark_{func_name.lower()}_ratio.png'))

# Filter for 'RollingMean' function
for func_name in df['func'].unique():
    print(func_name)
    df_filtered = df[df['func'] == func_name]
    make_func_plot(df_filtered, func_name)

