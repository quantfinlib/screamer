# `RollingSum`

## Description

The `RollingSum` class computes the sum of values within a specified moving window over a sequence of data. This rolling sum provides a dynamic aggregate of values in the window as it moves across the data, enabling analysis of trends over localized segments.

*Initial values*: The constructor requires a positive integer `window_size` parameter to define the rolling window.  
*NaN handling*: NaN values are not handled natively and should be preprocessed if necessary.

## Usage Example and Plot

```{eval-rst}
.. plotly::

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import RollingSum

    data = np.cumsum(np.random.normal(size=300))

    fig = make_subplots(
        rows=2, cols=1,
        shared_xaxes=True,
        row_heights=[2/3, 1/3],
        vertical_spacing=0.1
    )

    fig.add_trace(go.Scatter(y=data, mode='lines', name='Input Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=RollingSum(30)(data), mode='lines', name='Rolling Sum', line=dict(color='green')), row=2, col=1)

    fig.update_layout(
        title="Rolling Sum with Window Size 30",
        xaxis_title="Index",
        yaxis=dict(title="Input Data"),
        yaxis2=dict(title="Rolling Sum"),
        margin=dict(l=20, r=20, t=80, b=20),
        legend=dict(orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1)        
    )

    fig.show()
```

## Implementation Details

### Algorithm

`RollingSum` implements cyclic buffers to accumulate windowed statistics.

### Complexity

* **Time Complexity**: `O(log(1))` per new element due to the insertion and deletion operations in the heaps.
* **Space Complexity**: `O(window_size)`, as only elements within the current window are stored.

