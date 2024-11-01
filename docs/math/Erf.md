# `Erf`

## Description

The `Erf` class computes the error function of each element in a data sequence, useful in probability, statistics, and Gaussian-based models. It maps values onto the range [-1, 1].

*Parameters*: `Erf` takes no parameters.

*NaN handling*: `NaN` values are not modified.

## Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import Erf

    data = np.random.normal(size=30)
    erf_data = Erf()(data)

    fig = make_subplots(
        rows=2, cols=1,
        shared_xaxes=True,
        row_heights=[1/2, 1/2],
        vertical_spacing=0.1
    )

    fig.add_trace(go.Scatter(y=data, mode='lines+markers', name='Original Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=erf_data, mode='lines+markers', name='Error Function (erf)', line=dict(color='red')), row=2, col=1)

    fig.update_layout(
        title="Error Function Transformation (Erf)",
        xaxis_title="Index",
        yaxis_title="Original Data",
        yaxis2_title="Error Function"
    )

    fig.show()

```
