# `EwKurt`

## Description

`EwKurt` computes the exponentially weighted moving kurtosis, indicating the "tailedness" of the data distribution while emphasizing recent values.


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

    from screamer import EwKurt

    data = np.cumsum(np.random.normal(size=300))
    ewkurt_data = EwKurt(span=20)(data)

    fig = make_subplots(rows=2, cols=1, shared_xaxes=True, row_heights=[1/2, 1/2], vertical_spacing=0.1)

    fig.add_trace(go.Scatter(y=data, mode='lines', name='Original Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=ewkurt_data, mode='lines', name='EwKurt', line=dict(color='red')), row=2, col=1)

    fig.update_layout(
        title="Exponentially Weighted Moving Kurtosis",
        xaxis_title="Index",
        yaxis=dict(title="Original Data"),
        yaxis2=dict(title="EwKurt", range=[-1, 2]),
        margin=dict(l=20, r=20, t=40, b=20),
        legend=dict(orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1)
    )

    fig.show()
```


### Formula Details

`EwKurt` computes the exponentially weighted excess kurtosis recursively, with a bias correction that uses an effective sample size, $N_{eff}$, derived from the sum of weights. This approach ensures an unbiased estimate even as older observations' influence diminishes over time, aligning with Pandas’ convention for excess kurtosis.

Let:
- **`alpha`** be the smoothing factor, calculated from `com`, `span`, `halflife`, or specified directly, where `0 < alpha < 1`.

For each new data point $x_t$, `EwKurt` updates five cumulative sums, $S_x$, $S_{xx}$, $S_{xxx}$, $S_{xxxx}$, $S_w$, and $S_{ww}$, as follows:

1. Adjust **$S_x$**, the weighted sum of values, by retaining a fraction $(1 - \alpha)$ of the previous weighted sum and adding the new value:

$$
S_x = S_x \times (1 - \alpha) + x_t
$$

2. Adjust **$S_{xx}$**, the weighted sum of squared values, by retaining a fraction of the previous sum and adding the square of the new value:

$$
S_{xx} = S_{xx} \times (1 - \alpha) + x_t^2
$$

3. Adjust **$S_{xxx}$**, the weighted sum of cubed values, by retaining a fraction of the previous sum and adding the cube of the new value:

$$
S_{xxx} = S_{xxx} \times (1 - \alpha) + x_t^3
$$

4. Adjust **$S_{xxxx}$**, the weighted sum of fourth powers, by retaining a fraction of the previous sum and adding the fourth power of the new value:

$$
S_{xxxx} = S_{xxxx} \times (1 - \alpha) + x_t^4
$$

5. Adjust **$S_w$**, the cumulative weight, by retaining a fraction of the previous weight and adding a weight of $1$:

$$
S_w = S_w \times (1 - \alpha) + 1
$$

6. Adjust **$S_{ww}$**, the cumulative squared weight, by retaining a fraction $(1 - \alpha)^2$ of the previous squared weight and adding $1$:

$$
S_{ww} = S_{ww} \times (1 - \alpha)^2 + 1
$$

7. Calculate the effective sample size, $N_{eff}$, as:

$$
N_{eff} = \frac{S_w^2}{S_{ww}}
$$

8. Compute the exponentially weighted mean, $\text{EwMean}$, as:

$$
\text{EwMean} = \frac{S_x}{S_w}
$$

9. Compute the exponentially weighted variance, $\text{EwVar}$, with bias correction:

$$
\text{EwVar} = \left( \frac{S_{xx}}{S_w} - \left( \frac{S_x}{S_w} \right)^2 \right) \times \frac{N_{eff}}{N_{eff} - 1}
$$

10. Calculate the standard deviation, $\text{EwStd}$, as:

$$
\text{EwStd} = \sqrt{\text{EwVar}}
$$

11. Compute the fourth central moment, $m_4$, as:

$$
m_4 = \frac{S_{xxxx}}{S_w} - 4 \cdot \frac{S_x}{S_w} \cdot \frac{S_{xxx}}{S_w} + 6 \cdot \left( \frac{S_x}{S_w} \right)^2 \cdot \frac{S_{xx}}{S_w} - 3 \cdot \left( \frac{S_x}{S_w} \right)^4
$$

12. Calculate the raw kurtosis, $g_2$, as:

$$
g_2 = \frac{m_4}{\text{EwVar}^2}
$$

13. Finally, compute the exponentially weighted excess kurtosis with bias correction:

$$
\text{EwKurt} = \frac{N_{eff} \cdot (N_{eff} + 1) \cdot g_2 - 3 \cdot (N_{eff} - 1)^2}{(N_{eff} - 1) \cdot (N_{eff} - 2) \cdot (N_{eff} - 3)}
$$

This calculation uses $N_{eff}$ to correct for the effective sample size, ensuring that the excess kurtosis measure remains unbiased by scaling based on the decreasing influence of older values.
