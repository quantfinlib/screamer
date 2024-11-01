# `EwMean`

## Description

`EwMean` computes the exponentially weighted moving mean of a data sequence, applying more weight to recent data points and allowing for a smoother, lagged response. You specify the decay rate through `alpha`, calculated from `com`, `span`, `halflife`, or `alpha` itself, aligning with the Pandas `ewm` interface.

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
    from screamer import EwMean

    # Generate example data
    data = np.cumsum(np.random.normal(size=300))

    # Compute exponentially weighted mean with a span of 20
    ewmean_data = EwMean(span=20)(data)

    # Create subplots with original and transformed data
    fig = make_subplots(rows=2, cols=1, shared_xaxes=True, row_heights=[1/2, 1/2], vertical_spacing=0.1)

    fig.add_trace(go.Scatter(y=data, mode='lines', name='Original Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=ewmean_data, mode='lines', name='EwMean', line=dict(color='red')), row=2, col=1)

    fig.update_layout(
        title="Exponentially Weighted Moving Mean",
        xaxis_title="Index",
        yaxis=dict(title="Original Data"),
        yaxis2=dict(title="EwMean"),
        margin=dict(l=20, r=20, t=40, b=20)
    )

    fig.show()
```

### Formula Details

`EwMean` computes the exponentially weighted moving mean recursively. This calculation approach provides a fast, iterative update for each new value without recalculating the entire window, giving more weight to recent observations. The weighted mean formula aligns with the Pandas `ewm` interface by adjusting for the weighted contribution of each new element.


Let:
- **`alpha`** be the smoothing factor calculated from `com`, `span`, `halflife`, or specified directly, where `0 < alpha < 1`.

For each new data point $x_t$, `EwMean` updates two cumulative sums, $S_x$ and $S_w$, as follows:

1. Adjust $S_x$ by retaining a fraction $(1 - \alpha)$ of the previous weighted sum and then adding the new value $x_t$:
   
$$
S_x = S_x \times (1 - \alpha) + x_t
$$

1. Adjust $S_w$, the cumulative weight, by similarly retaining a fraction $(1 - \alpha)$ of the previous weight sum, then adding a weight of $1$:

$$
S_w = S_w \times (1 - \alpha) + 1
$$

3. Compute the exponentially weighted moving mean as:

$$
\text{EwMean} = \frac{S_x}{S_w}
$$

