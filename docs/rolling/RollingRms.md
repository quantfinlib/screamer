# `RollingRms`

## Description

The `RollingRms` class calculates the root mean square (RMS) of values within a specified moving window. This provides a measure of the magnitude of variation in the data, capturing both positive and negative values as positive magnitudes.

*Initial values*: The constructor requires a positive integer `window_size` parameter to define the rolling window.  
*NaN handling*: NaN values are not handled natively and should be preprocessed if necessary.

## Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import RollingRms

    data = np.cumsum(np.random.normal(size=300))

    fig = make_subplots(
        rows=2, cols=1,
        shared_xaxes=True,
        row_heights=[2/3, 1/3],
        vertical_spacing=0.1
    )

    fig.add_trace(go.Scatter(y=data, mode='lines', name='Input Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=RollingRms(30)(data), mode='lines', name='Rolling RMS', line=dict(color='teal')), row=2, col=1)

    fig.update_layout(
        title="Rolling RMS with Window Size 30",
        xaxis_title="Index",
        yaxis=dict(title="Input Data"),
        yaxis2=dict(title="Rolling RMS"),
        margin=dict(l=20, r=20, t=80, b=20),
        legend=dict(orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1)        
    )

    fig.show()
```

## Implementation Details

### Algorithm

`RollingRms` implements cyclic buffers to accumulate windowed statistics.

### Complexity

* **Time Complexity**: `O(log(1))` per new element due to the insertion and deletion operations in the heaps.
* **Space Complexity**: `O(window_size)`, as only elements within the current window are stored.

