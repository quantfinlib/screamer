# `Abs`

## Description

The `Abs` class computes the absolute value of each element in a data sequence, transforming all negative values to their positive counterparts. This function is useful in scenarios where only the magnitude of values is relevant, regardless of their sign.

*Parameters*: `Abs` takes no parameters.

*NaN handling*: `NaN` values are not modified by this function.

## Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import Abs

    # Generate example data with negative values
    data = np.random.normal(size=30) * 10
    abs_data = Abs()(data)

    fig = make_subplots(
        rows=2, cols=1,
        shared_xaxes=True,
        row_heights=[1/2, 1/2],
        vertical_spacing=0.1
    )

    fig.add_trace(go.Scatter(y=data, mode='lines+markers', name='Original Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=abs_data, mode='lines+markers', name='Absolute Value', line=dict(color='red')), row=2, col=1)

    fig.update_layout(
        title="Absolute Value Transformation (Abs)",
        xaxis_title="Index",
        yaxis_title="Original Data",
        yaxis2_title="Absolute Value",
        margin=dict(l=20, r=20, t=80, b=20),
        legend=dict(orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1)        
    )

    fig.show()

```
