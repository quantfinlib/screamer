# `Diff`

## Description

The `Diff` function calculates the difference between each element in a sequence and the element `delay` positions before it. This allows for quick derivation of trends or changes over a given interval.

*Equation*:

$$
y[i] = x[i] - x[i - \text{delay}]
$$

*Parameters*:

- `delay` (int): The number of steps backward to use for the difference calculation. Must be non-negative.

*NaN handling*: When `delay` exceeds the available data points at the start of the sequence, the resulting elements are set to `NaN`.

## Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import Diff

    data = np.random.normal(size=50)
    diff_data = Diff(3)(data)

    fig = make_subplots(rows=2, cols=1, shared_xaxes=True, row_heights=[1/2, 1/2], vertical_spacing=0.1)

    fig.add_trace(go.Scatter(y=data, mode='lines+markers', name='Original Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=diff_data, mode='lines+markers', name='Diff (3-step)', line=dict(color='green')), row=2, col=1)

    fig.update_layout(
        title="Difference Computation (3-step Delay)",
        xaxis_title="Index",
        yaxis_title="Original Data",
        yaxis2_title="Difference",
        margin=dict(l=20, r=20, t=60, b=20),
        legend=dict(orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1)
    )

    fig.show()
```

## Implementation Details

`Diff` uses a simple array subtraction operation with efficient memory handling to produce the difference between elements separated by the `delay`. For edge cases where data points are not available due to the `delay`, the function outputs `NaN` to maintain alignment.