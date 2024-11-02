# `Lag`

## Description

The `Lag` function returns the values from a sequence offset by a specified number of steps backward. It shifts each element in the input by `delay` positions, effectively providing past values for analysis or comparison.

*Equation*:

$$
y[i] = x[i - \text{delay}]
$$

*Parameters*:

- `delay` (int): The number of steps by which to shift the input. Must be non-negative.

*NaN handling*: When `delay` is larger than the available data at the beginning of the sequence, resulting elements are set to `NaN`.

## Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import Lag

    data = np.random.normal(size=50)
    lagged_data = Lag(5)(data)

    fig = make_subplots(rows=2, cols=1, shared_xaxes=True, row_heights=[1/2, 1/2], vertical_spacing=0.1)

    fig.add_trace(go.Scatter(y=data, mode='lines+markers', name='Original Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=lagged_data, mode='lines+markers', name='Lagged by 5', line=dict(color='red')), row=2, col=1)

    fig.update_layout(
        title="Lagged Data (5-step Delay)",
        xaxis_title="Index",
        yaxis_title="Original Data",
        yaxis2_title="Lagged Data",
        margin=dict(l=20, r=20, t=60, b=20),
        legend=dict(orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1)
    )

    fig.show()
```

## Implementation Details

The `Lag` function shifts the input array by the specified `delay`. Internally, it uses an efficient buffer to access elements and insert NaNs as placeholders when necessary.
