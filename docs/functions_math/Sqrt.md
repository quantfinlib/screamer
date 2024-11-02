# `Sqrt`

## Description

The `Sqrt` class computes the square root of each element in a data sequence. It’s often used to reduce the scale of data or to transform squared values back to their original units.

*Parameters*: `Sqrt` takes no parameters.

*NaN handling*: Negative values and `NaN` values are not modified, as the square root is undefined for negative numbers.

## Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import Sqrt

    data = np.abs(np.random.normal(size=30))
    sqrt_data = Sqrt()(data)

    fig = make_subplots(
        rows=2, cols=1,
        shared_xaxes=True,
        row_heights=[1/2, 1/2],
        vertical_spacing=0.1
    )

    fig.add_trace(go.Scatter(y=data, mode='lines+markers', name='Original Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=sqrt_data, mode='lines+markers', name='Square Root', line=dict(color='red')), row=2, col=1)

    fig.update_layout(
        title="Square Root Transformation (Sqrt)",
        xaxis_title="Index",
        yaxis_title="Original Data",
        yaxis2_title="Square Root",
        margin=dict(l=20, r=20, t=80, b=20),
        legend=dict(orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1)        
    )

    fig.show()

```