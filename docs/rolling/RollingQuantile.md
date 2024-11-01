Here is the documentation for `RollingQuantile`, including a detailed explanation of its behavior, usage example, and complexities:

---

# `RollingQuantile`

## Description

The `RollingQuantile` class computes a specified quantile within a moving window over a sequence of data. Given a `window_size` and `quantile`, the function returns an interpolated value that represents the `quantile` position within each window. This approach is useful for tracking data distribution changes over time or identifying outliers based on quantile thresholds. For example, a `quantile` of 0.5 would return the median, while 0.25 and 0.75 would yield the 25th and 75th percentiles, respectively. 

*Parameters*: 
- **`window_size`**: Specifies the size of the rolling window. The first `window_size` values are returned as `NaN` since a full window is required for calculation.
- **`quantile`**: Specifies the desired quantile, a value between 0 and 1, with 0.5 representing the median.

*NaN handling*: The first `window_size` values are returned as `NaN`, as they do not form a complete window. If `NaN` values are present within the data sequence, they will be ignored in the quantile calculation.

## Usage Example and Plot

Below is an example of using `RollingQuantile` to calculate the rolling 25th and 75th percentiles for a random dataset, with a plot illustrating the results.

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import RollingQuantile

    # Generate example data
    data = np.cumsum(np.random.normal(size=300))

    # Rolling 25th and 75th quantiles with a window size of 30
    quantile_25_data = RollingQuantile(window_size=30, quantile=0.25)(data)
    quantile_75_data = RollingQuantile(window_size=30, quantile=0.75)(data)

    # Create subplots with specified row heights and shared x-axis
    fig = make_subplots(
        rows=2, cols=1,
        shared_xaxes=True,
        row_heights=[1/2, 1/2],
        vertical_spacing=0.1
    )

    # Add traces for original data and quantiles
    fig.add_trace(go.Scatter(y=data, mode='lines', name='Input Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=quantile_25_data, mode='lines', name='Rolling 25th Percentile', line=dict(color='blue')), row=2, col=1)
    fig.add_trace(go.Scatter(y=quantile_75_data, mode='lines', name='Rolling 75th Percentile', line=dict(color='red')), row=2, col=1)

    # Update layout with titles and axis labels
    fig.update_layout(
        title="Rolling Quantile with Window Size 30 (25th and 75th Percentiles)",
        xaxis_title="Index",
        yaxis=dict(title="Input Data"),
        yaxis2=dict(title="Quantile Values")
    )

    fig.show()
```

## Implementation Details

### Algorithm

`RollingQuantile` is implemented using an **Order Statistic Tree**, which allows efficient insertion, deletion, and quantile selection operations within each rolling window. As new values enter the window, they are inserted into the tree while the oldest value is removed. This maintains the sorted structure required to compute quantiles on-the-fly, ensuring accurate quantile calculation even with dynamically changing data within each window.

### Complexity

The Order Statistic Tree provides the following complexities:

* **Time Complexity**: 
  - Insertion and deletion operations are `O(log(window_size))`.
  - Quantile selection is `O(log(window_size))` due to the tree’s ordered structure.
  - The overall time complexity per element is therefore `O(log(window_size))`.

* **Space Complexity**: 
  - `O(window_size)`, as only elements within the current window are stored in the tree at any given time.

### Performance

`RollingQuantile` is highly efficient for quantile calculations over large datasets or real-time data streams. The use of an Order Statistic Tree reduces the computational burden typically associated with dynamic quantile computation, making it well-suited for applications requiring fast and reliable quantile tracking across moving windows.