import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# Get the directory of the current script
script_dir = os.path.dirname(os.path.abspath(__file__))


def make_func_plot(df_filtered, func_name):
    # Sort by window_size and assign colors
    sorted_window_sizes = sorted(df_filtered['window_size'].unique())
    colors = {ws: f'C{i}' for i, ws in enumerate(sorted_window_sizes)}

    # Plot
    plt.figure(figsize=(6, 4))
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


# Read the data
results = pd.read_csv(
    os.path.join(script_dir, 'experiments', 'benchmark_results.csv'),
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
# Group by all columns except 'time' and reduce to the minimum 'time' for each group
results = results.groupby(['func', 'lib', 'var', 'window_size', 'n'], as_index=False).agg({'time': 'min'})

# sort 
results = results.sort_values(by=['func', 'lib', 'var', 'window_size', 'n'])


def plot1(screamer_data, ref_data, func, lib, var):
    ref_name = lib
    if len(var) > 0:
        ref_name += f' ({var})'

    fig, axs = plt.subplots(2, 1, figsize=(8,6), sharex=True)
    fig.subplots_adjust(right=0.75)

    for i, window_size in enumerate(ref_data['window_size'].unique()):
        a = screamer_data[screamer_data['window_size'] == window_size]
        b = ref_data[ref_data['window_size'] == window_size]

        # Merge on 'n' to get only rows where both DataFrames have the same 'n'
        merged = pd.merge(a[['n', 'time']], b[['n', 'time']], on='n', suffixes=('_a', '_b'))
        merged['time_ratio'] = merged['time_b'] / merged['time_a']
    

        axs[0].loglog(a['n'], a['time'], color=f'C{i}', marker='o', linestyle='-')
        axs[0].loglog(b['n'], b['time'], color=f'C{i}',  linestyle='-', label=f'{window_size:>10,.0f}'.replace(',', '.'))
        axs[1].semilogx(merged['n'], merged['time_ratio'], color=f'C{i}', linestyle='-',
              label=f'{window_size:>10,.0f}'.replace(',', '.'))
    #
    #  legends

    # Add the primary legend for Method A and Method B
    marker_line = plt.Line2D([], [], color='black', marker='o', linestyle='-', label='screamer')
    line_only = plt.Line2D([], [], color='black', linestyle='-', label=f'{ref_name}')
    first_legend = axs[0].legend(handles=[marker_line, line_only], loc='upper left',
                         title="Methods", bbox_to_anchor=(0.80, 0.93), bbox_transform=fig.transFigure)
    axs[0].add_artist(first_legend)  # Add this legend manually to avoid being replaced

    # Window size legend
    second_legend = axs[0].legend(loc='upper left', title="Window Sizes", bbox_to_anchor=(0.80, 0.8), bbox_transform=fig.transFigure)

    # axis labels
    axs[1].set_xlabel('data size')
    axs[0].set_ylabel('time')
    axs[1].set_ylabel('Speedup factor')

    # axis ranges, get the current y-axis limits
    _, y_max = axs[1].get_ylim()
    axs[1].set_ylim(0, max(y_max, 2))

    # title
    axs[0].text(0.02, 0.97, "timing (sec)", transform=axs[0].transAxes,
        ha='left', va='top', fontsize=12,
        bbox=dict(facecolor='white', alpha=0.5, edgecolor='none')
    )
    axs[1].text(0.02, 0.97, "speedup factor", transform=axs[1].transAxes,
        ha='left', va='top', fontsize=12,
        bbox=dict(facecolor='white', alpha=0.5, edgecolor='none')
    )

    plt.suptitle(f'{func}:  screamer v.s. {ref_name}', fontweight='bold')
    # misc
    for ax in axs:
        ax.grid(True)
    plt.tight_layout()    
    fig.subplots_adjust(hspace=0.1)  # Reduce hspace as needed        

    # save
    script_dir = os.path.dirname(os.path.abspath(__file__))
    plt.savefig(os.path.join(script_dir, 'plots', f'benchmark_{func}_{lib}_{var}.png'))
    plt.close()

# Loop over all screamer functions
screamer_funcs = results[results['lib'] == 'screamer']['func'].unique()

for func in screamer_funcs:

    # Benchmark data for this screamer function
    screamer_data = results[(results['lib'] == 'screamer') & (results['func'] == func)]

    # Find ref functions
    ref_funcs = results[(results['func'] == func) & (results['lib'] !=  'screamer')]

    if not ref_funcs.empty:

        # Get the lib and var of all the reference functions
        for lib, var in ref_funcs[['lib', 'var']].drop_duplicates().values:
            ref_data = results[(results['lib'] == lib) & (results['func'] == func) & (results['var'] == var)]
            plot1(screamer_data, ref_data, func, lib, var)

