# `Sign`

## Description

The `Sign` class computes the sign of each element in a data sequence, mapping each value to -1, 0, or +1. This function is useful for extracting the direction (positive or negative) of each value while discarding magnitude.

*Parameters*: `Sign` takes no parameters.

*NaN handling*: `NaN` values are not modified.

## Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import Sign

    data = np.random.normal(size=30)
    sign_data = Sign()(data)

    fig = make_subplots(
        rows=2, cols=1,
        shared_xaxes=True,
        row_heights=[1/2, 1/2],
        vertical_spacing=0.1
    )

    fig.add_trace(go.Scatter(y=data, mode='lines+markers', name='Original Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=sign_data, mode='lines+markers', name='Sign (-1, 0, +1)', line=dict(color='red')), row=2, col=1)

    fig.update_layout(
        title="Sign Transformation (Sign)",
        xaxis_title="Index",
        yaxis_title="Original Data",
        yaxis2_title="Sign",
        margin=dict(l=20, r=20, t=80, b=20),
        legend=dict(orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1)        
    )

    fig.show()

```