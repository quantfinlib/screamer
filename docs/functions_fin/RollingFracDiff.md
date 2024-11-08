# `RollingFracDiff`

## Description

`RollingFracDiff` computes the rolling fractional differentiation of a time series, retaining long-term memory while applying a fractional degree of differencing. This method is useful for transforming a time series to achieve stationarity while preserving as much of the memory as possible. Fractional differentiation allows for non-integer degrees of differencing.

### Formulas

For each new data point $x_t$, `RollingFracDiff` computes weights $w_k$ based on the desired degree of differentiation $d$ and convolves the input time series with these weights over a rolling window of size `window_size`.

1. **Weight Calculation**: Calculate weights $w_k$ recursively based on $d$:

$$
w_k = (-1)^k \cdot \binom{d}{k},
$$
which allows us to compute the weights in the following way:

$$
w_{0} = 1,
$$

$$
w_k = - \frac{d - k + 1}{k} w_{k-1}
$$

2. **Thresholding**: Any weight $w_k$ that falls below the specified `threshold` is ignored, effectively truncating the weight sequence.

3. **Rolling Fractional Differentiation**: For each point $t$, apply the computed weights over the rolling window, yielding the fractionally differentiated value $y_t$:

$$
y_t = \sum_{k=0}^{\min(t, \mathrm{window_{size}} - 1)} w_k \cdot x_{t - k}
$$

### Parameters

- **`window_size`**: Integer specifying the size of the rolling window. This is the number of past values used in the fractional differentiation calculation.
- **`frac_order`** (`d`): Float representing the degree of fractional differentiation. `d = 0` corresponds to an identity transformation of the data, while `d=1` results in standard difference. The higher the value of `d`, the lower the amount of memory preserved.
- **`threshold`**: Float specifying the minimum weight threshold. Any computed weight $w_k$ smaller than this threshold is set to zero. This optimizes computation by ignoring insignificant contributions.

## Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import RollingFracDiff

    # Generate example data
    data = np.cumsum(np.random.normal(size=1000))

    # Apply rolling fractional differentiation with window size 30, frac_order 0.5, threshold 0.001
    frac_diff_data = RollingFracDiff(window_size=30, frac_order=0.5, threshold=0.001)(data)

    # Create subplots with original and transformed data
    fig = make_subplots(rows=2, cols=1, shared_xaxes=True, row_heights=[1/2, 1/2], vertical_spacing=0.1)

    fig.add_trace(go.Scatter(y=data, mode='lines', name='Original Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=frac_diff_data, mode='lines', name='Fractional Differentiation', line=dict(color='blue')), row=2, col=1)

    fig.update_layout(
        title="Rolling Fractional Differentiation",
        xaxis_title="Index",
        yaxis=dict(title="Original Data"),
        yaxis2=dict(title="FracDiff Data"),
        margin=dict(l=20, r=20, t=80, b=20),
        legend=dict(orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1)
    )

    fig.show()
```