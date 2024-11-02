# `Exp`

## Description

The `Exp` class computes the exponential (e^x) of each element in a data sequence. This function is commonly used in exponential growth models and in scenarios requiring data scaling or transformations.

*Parameters*: `Exp` takes no parameters.

*NaN handling*: `NaN` values are not modified.

## Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import Exp

    data = np.random.normal(size=30) / 5
    exp_data = Exp()(data)

    fig = make_subplots(
        rows=2, cols=1,
        shared_xaxes=True,
        row_heights=[1/2, 1/2],
        vertical_spacing=0.1
    )

    fig.add_trace(go.Scatter(y=data, mode='lines+markers', name='Original Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=exp_data, mode='lines+markers', name='Exponential (e^x)', line=dict(color='red')), row=2, col=1)

    fig.update_layout(
        title="Exponential Transformation (Exp)",
        xaxis_title="Index",
        yaxis_title="Original Data",
        yaxis2_title="Exponential",
        margin=dict(l=20, r=20, t=80, b=20),
        legend=dict(orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1)        
    )

    fig.show()

```