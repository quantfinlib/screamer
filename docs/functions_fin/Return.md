# `Return`

## Description

The `Return` function computes the simple return between an element and the element `delay` positions before it. This function is commonly used to measure the relative change between data points, facilitating analysis of growth or decline over a specified interval.

*Equation*:

$$
y[i] = \frac{x[i] - x[i - \text{delay}]}{x[i - \text{delay}]}
$$

*Parameters*:

- `delay` (int): The number of steps backward to use for calculating the return. Must be non-negative.

*NaN handling*: When `delay` exceeds the available data points at the start of the sequence, or if `x[i - \text{delay}]` equals zero (to avoid division by zero), the output is set to `NaN`.

## Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import Return

    data = np.random.normal(1, 0.1, size=50)
    return_data = Return(5)(data)

    fig = make_subplots(rows=2, cols=1, shared_xaxes=True, row_heights=[1/2, 1/2], vertical_spacing=0.1)

    fig.add_trace(go.Scatter(y=data, mode='lines+markers', name='Original Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=return_data, mode='lines+markers', name='Return (5-step)', line=dict(color='blue')), row=2, col=1)

    fig.update_layout(
        title="Simple Return Computation (5-step Delay)",
        xaxis_title="Index",
        yaxis_title="Original Data",
        yaxis2_title="Simple Return",
        margin=dict(l=20, r=20, t=60, b=20),
        legend=dict(orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1)
    )

    fig.show()
```

## Implementation Details

The `Return` function uses array operations to efficiently compute the relative change between data points. To handle cases where the denominator could be zero, conditional checks ensure that the output is set to `NaN` when such divisions are encountered.
