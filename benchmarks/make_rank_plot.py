import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import pandas as pd
from benchmarks import read_expiriments
import os

df = read_expiriments()
print(df)


df = df.groupby(['func', 'lib', 'var'], as_index=False).agg({'time': 'mean'})
df = df.groupby(['func', 'lib'], as_index=False).agg({'time': 'min'})
print(df)


# Pivot `ours` to columns with `time` as values
df = df.pivot(index="func", columns="lib", values="time")
print(df)
df = df.div(df['screamer'], axis=0)
df = df.drop(columns='screamer')
print(df)

# Sort rows by the smallest value in each row
df = df.assign(min_value=df.min(axis=1)).sort_values(by="min_value").drop(columns="min_value")


# ----------------------------------------------------------------------------------
# Plotting
# ----------------------------------------------------------------------------------
# Define color mapping for each `lib_non_ours` value

# Define colors for each column
colors = {
    "numpy": "lightblue",
    "pandas": "steelblue",
}



# Set threshold for the break point
break_threshold = 8.5
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

                # Add a break symbol (tilde) to indicate the gap
                #ax.text(break_threshold - 0.5, bar_position, '~', ha='center', va='center', fontsize=12, color='black')

            # Annotate with the value
            if value <= break_threshold:
                text_pos = value
            else:
                text_pos = break_threshold + 0.2*np.log(value - break_threshold)
            ax.text(
                text_pos + 0.2, 
                bar_position, 
                f"{(value - 1) * 100:+,.0f}%", 
                va='center', 
                fontsize=8
            )

# Customize plot appearance
ax.set_yticks(positions + bar_width)  # Center tick labels between grouped bars
ax.set_yticklabels(df.index)          # Set function names as y-axis labels
ax.set_xlabel("Speedup factor")                # Label for x-axis
ax.set_title("Screamer speedup")

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
legend_patches = [plt.Line2D([0], [0], color=color, lw=4, label=col) for col, color in colors.items()]
ax.legend(handles=legend_patches, loc='lower right',  frameon=False)

# Get the directory of the current script
script_dir = os.path.dirname(os.path.abspath(__file__))

plt.savefig(os.path.join(script_dir, 'plots', f'rank_plot.png'))
plt.close()
