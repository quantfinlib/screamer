# `Erfc`

## Description

The `Erfc` class computes the complementary error function (1 - erf(x)) for each element in the data sequence, often used in Gaussian models and probability.

*Parameters*: `Erfc` takes no parameters.

*NaN handling*: `NaN` values are not modified.

## Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import Erfc

    data = np.random.normal(size=30)
    erfc_data = Erfc()(data)

    fig = make_subplots(
        rows=2, cols=1,
        shared_xaxes=True,
        row_heights=[1/2, 1/2],
        vertical_spacing=0.1
    )

    fig.add_trace(go.Scatter(y=data, mode='lines+markers', name='Original Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=erfc_data, mode='lines+markers', name='Compl. Error Function (erfc)', line=dict(color='red')), row=2, col=1)

    fig.update_layout(
        title="Complementary Error Function Transformation (Erfc)",
        xaxis_title="Index",
        yaxis_title="Original Data",
        yaxis2_title="Complementary Error Function",
        margin=dict(l=20, r=20, t=80, b=20),
        legend=dict(orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1)        
    )

    fig.show()

```
