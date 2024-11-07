To document `RollingOU`, we'll first examine the behavior and interpretation of the mean reversion rate (mrr) parameter in a discrete-time Ornstein-Uhlenbeck (OU) process. This parameter controls the extent to which the model's values revert toward the mean. A high mrr value close to 1 indicates strong mean reversion, with values quickly adjusting back to the mean. When mrr is greater than 1, the model can display oscillatory or unstable behavior. If mrr is less than zero, the process instead diverges from the mean, a phenomenon referred to as "mean repulsion."

A refined interpretation of mrr is as follows:

- **Positive mrr (0 < mrr < 1)**: Indicates mean reversion, where values tend to return toward the mean at a rate governed by mrr. The closer mrr is to 1, the faster the reversion to the mean.
- **Negative mrr (mrr < 0)**: Represents mean repulsion, where values tend to move away from the mean rather than reverting to it.
- **High mrr (mrr > 1)**: Indicates oscillatory or unstable behavior, as the process diverges around the mean, leading to amplified oscillations or instability.

With this understanding, we can move to the documentation.

---

# `RollingOU`

## Description

`RollingOU` applies a rolling discrete-time Ornstein-Uhlenbeck (OU) model to a data series within a specified window. The OU model, commonly used to model mean-reverting processes, estimates the rate of reversion to the mean and the variance of the noise in each window. As a rolling estimator, `RollingOU` recalculates model parameters at each step based on a sliding window, making it suitable for non-stationary time series where parameters may evolve over time.

The discretized form of the OU model is:

$$
x[i] = \mu - (x[i-1] - \mu) \cdot \text{mrr} + \sigma \cdot \epsilon
$$

where $ x[i] $ is the current value, $ x[i-1] $ is the previous value, $ \mu $ is the mean, $ \text{mrr} $ is the mean reversion rate, $ \sigma $ is the standard deviation of the noise, and $ \epsilon \sim \mathcal{N}(0,1) $ is a standard normal random variable. The parameter `mrr` controls the rate and nature of mean reversion:

- **0 < mrr < 1**: Values tend to revert to the mean at a rate determined by mrr, with a higher mrr closer to 1 indicating faster mean reversion.
- **mrr < 0**: Values display mean repulsion, where deviations from the mean amplify rather than attenuate.
- **mrr > 1**: Oscillatory behavior or instability arises, as the model overshoots the mean.

### Parameters

**`window_size`** *(int)*: The size of the rolling window used to fit the OU model. A larger window provides a smoother estimate but may be less responsive to rapid changes.

**`output`** *(int)*: Specifies the output type:
- **0**: Returns the mean reversion rate (mrr).
- **1**: Returns the estimated mean ($\mu$).
- **2**: Returns the relative mean ($\mu^\prime = \mu - x[i]$), the mean relative to the last value.
- **3**: Returns the estimated standard deviation of the noise ($\sigma$).

*NaN handling*: NaN values are ignored within the window for estimation.

## Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import RollingOU

    # Generate synthetic data with cumulative noise
    np.random.seed(0)
    data = np.cumsum(np.random.normal(0, 1, 500))

    # Define a rolling OU model with window size of 50
    rolling_ou_mrr = RollingOU(window_size=50, output=0)
    rolling_ou_mean = RollingOU(window_size=50, output=1)
    rolling_ou_std = RollingOU(window_size=50, output=3)

    # Apply RollingOU on the data for each output type
    mrr_estimates = [rolling_ou_mrr(d) for d in data]
    mean_estimates = [rolling_ou_mean(d) for d in data]
    std_estimates = [rolling_ou_std(d) for d in data]

    # Create subplots with original and estimated parameters
    fig = make_subplots(rows=4, cols=1, shared_xaxes=True, 
                        row_heights=[1/4, 1/4, 1/4, 1/4], vertical_spacing=0.05)

    fig.add_trace(go.Scatter(y=data, mode='lines', name='Original Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=mrr_estimates, mode='lines', name='Mean Reversion Rate (mrr)'), row=2, col=1)
    fig.add_trace(go.Scatter(y=mean_estimates, mode='lines', name='Estimated Mean (μ)', line=dict(color='green')), row=3, col=1)
    fig.add_trace(go.Scatter(y=std_estimates, mode='lines', name='Noise Std Dev (σ)', line=dict(color='purple')), row=4, col=1)

    fig.update_layout(
        title="Rolling Discrete-Time Ornstein-Uhlenbeck Model Parameters",
        xaxis_title="Index",
        yaxis=dict(title="Original Data"),
        yaxis2=dict(title="mrr"),
        yaxis3=dict(title="mean"),
        yaxis4=dict(title="noise"),
        margin=dict(l=20, r=20, t=120, b=20),
        legend=dict(orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1)        
    )

    fig.show()
```

### Formula Details

The `RollingOU` function estimates parameters for the OU model in a rolling window. For a window of data $ \{x[i - W], \dots, x[i]\} $, it calculates:

1. **Mean Reversion Rate (mrr)**: The rate at which the model reverts to the mean is estimated from the correlation between current and prior values. For $ \text{mrr} > 0 $, the process exhibits mean-reverting behavior, with values tending to return to the mean. Negative values imply mean repulsion, while values greater than 1 indicate oscillatory or potentially unstable behavior.

2. **Realtive mean ($\mu$)**: The model estimates the long-term mean by extrapolating the model, giving the steady-state value toward which the process reverts in the absence of noise. 

3. **Realtive mean ($\mu^\prime$)**: The relative mean is expressed relative to the last observation $\mu^\prime = \mu - x[i]$. A positive relative means indicated that the mean is above the current signal value.

4. **Standard Deviation ($\sigma$)**: This parameter estimates the noise level by calculating the standard deviation of residuals within the window, capturing the typical magnitude of deviations around the mean.


where $ \epsilon $ is drawn from a standard normal distribution. This iterative update leverages a rolling window for dynamic re-estimation of the parameters, allowing adaptation to time-varying behavior in the data.