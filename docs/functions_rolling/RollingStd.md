# `RollingStd`

## Description

The `RollingStd` class computes the sample standard deviation of values within a moving window, quantifying the typical spread or dispersion from the mean. It’s useful for tracking variability in the data over a localized window.

*Initial values*: The constructor requires a positive integer `window_size` parameter to define the rolling window.  
*NaN handling*: NaN values are not handled natively and should be preprocessed if necessary.

## Usage Example and Plot

```{eval-rst}
.. plotly::

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import RollingStd

    data = np.cumsum(np.random.normal(size=300))

    fig = make_subplots(
        rows=2, cols=1,
        shared_xaxes=True,
        row_heights=[2/3, 1/3],
        vertical_spacing=0.1
    )

    fig.add_trace(go.Scatter(y=data, mode='lines', name='Input Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=RollingStd(30)(data), mode='lines', name='Rolling Std Deviation', line=dict(color='blue')), row=2, col=1)

    fig.update_layout(
        title="Rolling Standard Deviation with Window Size 30",
        xaxis_title="Index",
        yaxis=dict(title="Input Data"),
        yaxis2=dict(title="Rolling Std Deviation"),
        margin=dict(l=20, r=20, t=80, b=20),
        legend=dict(orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1)        
    )

    fig.show()
```

## Implementation Details

### Algorithm

`RollingStd` implements cyclic buffers to accumulate windowed statistics.

### Complexity

* **Time Complexity**: `O(log(1))` per new element due to the insertion and deletion operations in the heaps.
* **Space Complexity**: `O(window_size)`, as only elements within the current window are stored.

