# `RollingMedian`

## Description
The `RollingMedian` class computes the median value within a moving window of specified size over a sequence of data. 


*Initial values*: The constructor requires a positive integer `window_size` parameter to define the rolling window.  
*NaN handling*: NaN values are not handled natively and should be preprocessed if necessary.

## Usage Example and Plot
Below is an example of using `RollingMedian` to calculate the rolling median for a random dataset, along with a plot illustrating its output.

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from screamer import RollingMedian

    # Generate example data
    N = 300
    window_size = 30
    data = np.cumsum(np.random.normal(size=300))

    # Apply rolling maximum calculation
    rolling_median = RollingMedian(window_size)
    results = rolling_median(data)

    # Plotting with Plotly
    fig = go.Figure()
    fig.add_trace(go.Scatter(y=data, mode='lines', name='Input Data'))
    fig.add_trace(go.Scatter(y=results, mode='lines', name='Rolling Median', line=dict(color='red')))
    fig.update_layout(title=f"Rolling median with Window Size = {window_size}",
                    xaxis_title="Index",
                    yaxis_title="Value")
    fig.show()
```


## Implementation Details

### Algorithm

`RollingMedian` implements a two-heap algorithm using a max-heap (`low`) for the lower half of values and a min-heap (`high`) for the upper half, allowing for efficient median calculation over a sliding window. When a new value is added, it is inserted into the appropriate heap based on its value relative to the current median. After insertion, the algorithm rebalances the heaps to ensure that the difference in their sizes is at most one. This setup enables quick median retrieval: if the heaps are equal in size, the median is the average of the largest value in `low` and the smallest in `high`; if they differ, the median is the top of `low`.

### Complexity

* **Time Complexity**: `O(log(window_size))` per new element due to the insertion and deletion operations in the heaps.
* **Space Complexity**: `O(window_size)`, as only elements within the current window are stored.


### Performance

* Short streams (n=1.000): 150% faster than `Pandas Rolling median`
* Longer streams (n=1.000.000): 50% faster than `Pandas Rolling median`
