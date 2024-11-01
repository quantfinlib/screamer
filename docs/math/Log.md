# `Log`

## Description

The `Log` class computes the natural logarithm (ln) of each element in a data sequence. This function is useful for logarithmic scaling, often employed to stabilize variance or compress large data ranges.

*Parameters*: `Log` takes no parameters.

*NaN handling*: `NaN` values and negative values (since they’re undefined for logarithms) are not modified and remain as `NaN`.

## Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import Log

    data = np.abs(np.random.normal(size=30)) + 1
    log_data = Log()(data)

    fig = make_subplots(
        rows=2, cols=1,
        shared_xaxes=True,
        row_heights=[1/2, 1/2],
        vertical_spacing=0.1
    )

    fig.add_trace(go.Scatter(y=data, mode='lines+markers', name='Original Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=log_data, mode='lines+markers', name='Natural Log (ln)', line=dict(color='red')), row=2, col=1)

    fig.update_layout(
        title="Natural Logarithm Transformation (Log)",
        xaxis_title="Index",
        yaxis_title="Original Data",
        yaxis2_title="Natural Log"
    )

    fig.show()
```
