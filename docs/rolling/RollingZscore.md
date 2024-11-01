# `RollingZscore`

## Description

The `RollingZscore` class computes the rolling z-score for each value in a specified moving window, representing how many standard deviations each data point is from the mean of its window. This metric is useful for identifying outliers or deviations in a sequence.

*Initial values*: The constructor requires a positive integer `window_size` parameter to define the rolling window.  
*NaN handling*: NaN values are not handled natively and should be preprocessed if necessary.

## Usage Example and Plot

```{eval-rst}
.. plotly::

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import RollingZscore

    data = np.cumsum(np.random.normal(size=300))

    fig = make_subplots(
        rows=2, cols=1,
        shared_xaxes=True,
        row_heights=[2/3, 1/3],
        vertical_spacing=0.1
    )

    fig.add_trace(go.Scatter(y=data, mode='lines', name='Input Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=RollingZscore(30)(data), mode='lines', name='Rolling Z-score', line=dict(color='red')), row=2, col=1)

    fig.update_layout(
        title="Rolling Z-score with Window Size 30",
        xaxis_title="Index",
        yaxis=dict(title="Input Data"),
        yaxis2=dict(title="Rolling Z-score")
    )

    fig.show()
```


## Implementation Details

### Algorithm

`RollingZscore` implements cyclic buffers to accumulate windowed statistics.

### Complexity

* **Time Complexity**: `O(log(1))` per new element due to the insertion and deletion operations in the heaps.
* **Space Complexity**: `O(window_size)`, as only elements within the current window are stored.
