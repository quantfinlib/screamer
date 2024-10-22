import numpy as np
import os
import timeit
from screamer import SMA, SMAL
import matplotlib.pyplot as plt

# Define a helper function to time a function call
def time_function(func, *args):
    start_time = timeit.default_timer()  # Record start time
    result = func(*args)  # Call the function
    end_time = timeit.default_timer()  # Record end time
    return result, end_time - start_time  # Return result and time taken

fig, axs = plt.subplots(2,1, figsize=(8,6), sharex=True)

for i, delay in enumerate([10, 100, 1000, 10_000]):
    print(i, 'delay', delay)

    Ns = []
    time1s = []
    time2s = []

    for N_ in [100, 1000, 10_000, 100_000, 1_000_000, 10_000_000, 100_000_000]:
        for n_factor in [1,2,5]:
            N = N_ * n_factor

            if N > 200_000_000:
                continue

            if delay > 0.9 * N:
                continue

            x = np.random.normal(size=N)

            # Variant 1 (Lag)
            lag1 = SMA(delay)
            y1, time1 = time_function(lag1.transform, x)  # Time variant 1
            time1s.append(time1)

            # Variant 2 (Lagq)
            lag2 = SMAL(delay)
            y2, time2 = time_function(lag2.transform, x)  # Time variant 2
            time2s.append(time2)

            Ns.append(N)

            # Output the results
            print(f"N={N}, delay={delay}, time1={time1:.6f}s, time2={time2:.6f}s")
        
    axs[0].loglog(Ns, time1s, color=f'C{i}', label=f'sma({delay})')
    axs[0].loglog(Ns, time2s, color=f'C{i}', ls='--', label=f'smal({delay})')
    axs[1].semilogx(Ns, np.array(time2s) / np.array(time1s) - 1, color=f'C{i}', label=f'smal({delay})')

axs[0].grid()
axs[0].set_xlabel('input length')
axs[0].set_ylabel('time (sec)')
axs[0].legend()
axs[0].set_title('Timing SMAL vs SMA')

axs[1].grid()
axs[1].set_xlabel('input length')
axs[1].set_ylabel('extra time pct SMAL')
axs[1].legend()
axs[1].set_title('Timing SMAL/SMA - 1')

plt.tight_layout()


# Get the directory of the current script
script_dir = os.path.dirname(os.path.abspath(__file__))

# Define the path to save the plot in the same directory as the script
save_path = os.path.join(script_dir, 'time_sma_vs_smal.png')

# Save the plot
plt.savefig(save_path)

