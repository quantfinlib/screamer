# `EwZscore`

## Description

`EwZscore` computes the exponentially weighted moving z-score, which standardizes data based on an exponentially weighted mean and standard deviation, identifying outliers and deviations in real-time.


### Parameters

One of the following decay parameters is required to calculate `alpha`, where a higher `alpha` value gives recent points more influence:

- **`com`**: Center of mass. `alpha = 1 / (1 + com)`
- **`span`**: Span. `alpha = 2 / (span + 1)`
- **`halflife`**: Half-life. `alpha = 1 - exp(-log(2) / halflife)`
- **`alpha`**: Directly specifies the smoothing factor, where `0 < alpha < 1`

*NaN handling*: NaN values are ignored in the mean calculation.

### Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import EwZscore

    data = np.cumsum(np.random.normal(size=300))
    ewzscore_data = EwZscore(span=20)(data)

    fig = make_subplots(
        rows=2, cols=1,
        shared_xaxes=True,
        row_heights=[1/2, 1/2],
        vertical_spacing=0.1
    )

    fig.add_trace(go.Scatter(y=data, mode='lines', name='Original Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=ewzscore_data, mode='lines', name='EwZscore', line=dict(color='red')), row=2, col=1)

    fig.update_layout(
        title="Exponentially Weighted Moving Z-Score",
        xaxis_title="Index",
        yaxis=dict(title="Original Data"),
        yaxis2=dict(title="EwZscore", range=[-4, 4]),
        margin=dict(l=20, r=20, t=40, b=20)
    )

    fig.show()
```