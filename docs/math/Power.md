# `Power`

## Description

The `Power` class computes the power of each element in a data sequence. 


*Equation*:

$$
f(x) = x^p 
$$

*Parameters*:

- `p` (double): Exponent of the power function.

*NaN handling*: `NaN` values are not modified by this function.

## Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import Power

    data = np.random.normal(size=30)
    result = Power(2)(data)

    fig = make_subplots(
        rows=2, cols=1,
        shared_xaxes=True,
        row_heights=[1/2, 1/2],
        vertical_spacing=0.1
    )

    fig.add_trace(go.Scatter(y=data, mode='lines+markers', name='Original Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=result, mode='lines+markers', name='Power of 2, square', line=dict(color='red')), row=2, col=1)

    fig.update_layout(
        title="Power 2, square",
        xaxis_title="Index",
        yaxis_title="Original Data",
        yaxis2_title="Square",
        margin=dict(l=20, r=20, t=80, b=20),
        legend=dict(orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1)        
    )

    fig.show()
```
