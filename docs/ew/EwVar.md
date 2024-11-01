# `EwVar`

## Description

`EwVar` computes the exponentially weighted moving variance, tracking the variability of recent data while giving more weight to the latest values. Specify decay through `alpha`, derived from `com`, `span`, `halflife`, or `alpha` itself.

### Parameters

You must specify one of the following parameters to calculate `alpha`:

- **`com`**: Center of mass. `alpha = 1 / (1 + com)`
- **`span`**: Span. `alpha = 2 / (span + 1)`
- **`halflife`**: Half-life. `alpha = 1 - exp(-log(2) / halflife)`
- **`alpha`**: Directly sets the smoothing factor, `0 < alpha < 1`

*NaN handling*: NaN values are ignored in the variance calculation.

### Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import EwVar

    # Generate example data
    data = np.cumsum(np.random.normal(size=300))

    # Compute exponentially weighted variance with a span of 20
    ewvar_data = EwVar(span=20)(data)

    fig = make_subplots(
        rows=2, cols=1,
        shared_xaxes=True,
        row_heights=[1/2, 1/2],
        vertical_spacing=0.1
    )

    fig.add_trace(go.Scatter(y=data, mode='lines', name='Original Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=ewvar_data, mode='lines', name='EwVar', line=dict(color='red')), row=2, col=1)

    fig.update_layout(
        title="Exponentially Weighted Moving Variance",
        xaxis_title="Index",
        yaxis=dict(title="Original Data"),
        yaxis2=dict(title="EwVar", range=[0, 25]),
        margin=dict(l=20, r=20, t=40, b=20)
    )

    fig.show()
```

### Formula Details

`EwVar` computes the exponentially weighted moving variance recursively, with a bias correction that uses an effective sample size, $N_{eff}$, calculated from the sum of weights. This ensures an unbiased estimate even as the weighting of older observations diminishes over time.

Let:
- **`alpha`** be the smoothing factor, calculated from `com`, `span`, `halflife`, or specified directly, where `0 < alpha < 1`.

For each new data point $x_t$, EwVar updates four cumulative sums, $S_x$, $S_{xx}$, $S_w$, and $S_{ww}$, as follows:

1. Adjust **$S_x$**, the weighted sum of values, by retaining a fraction $(1 - \alpha)$ of the previous weighted sum and adding the new value:

$$
S_x = S_x \times (1 - \alpha) + x_t
$$

2. Adjust **$S_{xx}$**, the weighted sum of squared values, by retaining a fraction of the previous sum and adding the square of the new value:

$$
S_{xx} = S_{xx} \times (1 - \alpha) + x_t^2
$$

3. Adjust **$S_w$**, the cumulative weight, by retaining a fraction of the previous weight and adding a weight of $1$:

$$
S_w = S_w \times (1 - \alpha) + 1
$$

1. Adjust **$S_{ww}$**, the cumulative squared weight, by retaining a fraction $(1 - \alpha)^2$ of the previous squared weight and adding $1$:

$$
S_{ww} = S_{ww} \times (1 - \alpha)^2 + 1
$$

5. Calculate the effective sample size, $N_{eff}$, as:

$$
N_{eff} = \frac{S_w^2}{S_{ww}}
$$

6. Compute the exponentially weighted mean, $\text{EwMean}$, as:

$$
\text{EwMean} = \frac{S_x}{S_w}
$$

7. Finally, calculate the exponentially weighted moving variance as:

$$
\text{EwVar} = \left( \frac{S_{xx}}{S_w} - \left( \frac{S_x}{S_w} \right)^2 \right) \times \frac{N_{eff}}{N_{eff} - 1}
$$

The term $N_{eff}$ compensates for the decreasing influence of older values, ensuring that the variance remains unbiased by scaling based on the weighted sample size.

