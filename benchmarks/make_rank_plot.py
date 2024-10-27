import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import pandas as pd
from benchmarks import read_expiriments
import os

df = read_expiriments()
print(df)

# get largest n size
#max_n = df['n'].max()
#df = df[df['n']==max_n]

# Compute average time (across all delays/window sizes)
df = df.groupby(['func', 'lib', 'var'], as_index=False).agg({'time': 'mean'})

# If we have multiple implementation in a specific lib, use the fastest
df = df.groupby(['func', 'lib'], as_index=False).agg({'time': 'min'})

# Pivot 
df = df.pivot(index="func", columns="lib", values="time")
print(df)

# Find the best alternative
df['min_non_screamer'] = df.drop('screamer', axis=1).min(axis=1, skipna=True)

# Compute relative to the best alternative
df = df.div(df['min_non_screamer'], axis=0)
df = 1/df
df = df.drop('min_non_screamer', axis=1)

df = df.sort_values(by="screamer")
print(df)


# ----------------------------------------------------------------------------------
# Plotting
# ----------------------------------------------------------------------------------
# Define color mapping for each `lib_non_ours` value

# Define colors for each column
colors = {
    "numpy": "skyblue",
    "pandas": "steelblue",
    "screamer": "green",
}

text_colors = {
    "numpy": "steelblue",
    "pandas": "steelblue",
    "screamer": "green",
}


# Set threshold for the break point
break_threshold = 4
bar_width = 0.3

fig, ax = plt.subplots(figsize=(10, 6))
positions = np.arange(len(df.index))

for i, (col, color) in enumerate(colors.items()):
    for j, (func, value) in enumerate(df[col].items()):
        
        # Determine positions for the two parts of each bar
        bar_position = positions[j] + i * bar_width
        
        if pd.notna(value):
            
            # Part below threshold
            part_below = min(value, break_threshold)
            ax.barh(bar_position, part_below, height=bar_width, color=color)

            # Part above threshold
            if value > break_threshold:
                part_above = 0.2*np.log(value - break_threshold)
                ax.barh(
                    bar_position, part_above, height=bar_width, color=color,
                    left=break_threshold, hatch='//'  # Hatch pattern to indicate continuation
                )

            # Annotate with the value
            if np.abs(value - 1.0) > 0.01:
                if value <= break_threshold:
                    text_pos = value
                else:
                    text_pos = break_threshold + 0.2*np.log(value - break_threshold)
                ax.text(
                    text_pos + 0.05, 
                    bar_position, 
                    f"{(value - 1) * 100:+,.0f}%", 
                    va='center', 
                    fontsize=9,
                    color=text_colors[col]
                )

# Customize plot appearance
ax.set_yticks(positions + bar_width)  # Center tick labels between grouped bars
ax.set_yticklabels(df.index)          # Set function names as y-axis labels
ax.set_xlabel("Speedup factor")                # Label for x-axis
ax.set_title("Screamer speedup versus best alternative")

# Set x-axis to logarithmic scale
#ax.set_xscale('log')
ax.axvline(x=1, color='black', linestyle='--', linewidth=1, alpha=.25)

# Remove all spines except the bottom one
ax.spines['top'].set_visible(False)
ax.spines['right'].set_visible(False)
ax.spines['left'].set_visible(False)

# Add a white rectangle to create the gap at x=1
gap_x = break_threshold
gap_width = 0.2  # Width of the gap
ax.add_patch(plt.Rectangle((gap_x - gap_width / 2, .5), gap_width, len(df) + 2, color='white', zorder=5))

# Adjust x-ticks to stop at the break_threshold
ax.set_xticks(np.arange(1+int(break_threshold)))

# Add legend at the top below the title
legend_patches = [plt.Line2D([0], [0], color=color, lw=4, label=col) for col, color in colors.items()][::-1]
ax.legend(handles=legend_patches, loc='lower right',  frameon=False)

# Get the directory of the current script
script_dir = os.path.dirname(os.path.abspath(__file__))
plt.tight_layout()
plt.savefig(os.path.join(script_dir, 'plots', f'rank_plot.png'))
plt.close()
