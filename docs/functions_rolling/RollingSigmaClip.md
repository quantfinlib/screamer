# `RollingSigmaClip`

## Description

The `RollingSigmaClip` class performs rolling statistical clipping on a data sequence. It computes the rolling mean and standard deviation within a specified window and clips data points that fall outside dynamically calculated bounds based on z-scores. Specifically, it replaces values that exceed the bounds with the boundary values and excludes these clipped values from subsequent mean and standard deviation calculations. This method is useful for smoothing data, removing outliers, and preparing data for further analysis, especially when dealing with censored data from a truncated normal distribution.

*Parameters*:

- **`window_size`**: *(int)*  
  The size of the moving window over which the rolling statistics are computed. Must be a positive integer.

- **`lower`**: *(optional, float)*  
  The lower z-score threshold for clipping. Data points with z-scores below this threshold are set to the lower bound. If unspecified, there is no lower clipping based on z-score.

- **`upper`**: *(optional, float)*  
  The upper z-score threshold for clipping. Data points with z-scores above this threshold are set to the upper bound. If unspecified, there is no upper clipping based on z-score.

- **`output`**: *(optional, int)*  
  Determines the type of output returned by the function:
  - `0`: Returns the clipped data sequence.
  - `1`: Returns the rolling mean.
  - `2`: Returns the rolling standard deviation.
  - `3`: Returns the original data with outliers (beyond the bounds) replaced by `NaN`.

*NaN handling*: NaN values are propagated through the calculations. If NaN values are present in the input data, they will result in NaN outputs for the corresponding positions.

## Usage Example and Plot

Below is an example of using `RollingSigmaClip` to clip data points that are more than 2 standard deviations away from the rolling mean within a window size of 30. The plot illustrates the input data, clipped data, rolling mean, and upper and lower bounds.

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import RollingSigmaClip

    # Generate example data with outliers
    np.random.seed(42)
    data = np.random.normal(loc=0, scale=1, size=300)
    data[::50] += np.random.normal(loc=0, scale=10, size=6)  # Introduce outliers

    # Rolling Clipping Settings
    window_size = 30
    lower_z = -3  # Lower z-score threshold
    upper_z = 3   # Upper z-score threshold

    # Get the clipped data
    clipped_data = RollingSigmaClip(window_size=window_size, lower=lower_z, upper=upper_z)(data)
    rolling_mean = RollingSigmaClip(window_size=window_size, lower=lower_z, upper=upper_z, output=1)(data)
    rolling_std = RollingSigmaClip(window_size=window_size, lower=lower_z, upper=upper_z, output=2)(data)

    # Calculate upper and lower bounds
    upper_bound = rolling_mean + upper_z * rolling_std
    lower_bound = rolling_mean + lower_z * rolling_std

    # Create subplots
    fig = make_subplots(rows=2, cols=1, shared_xaxes=True, vertical_spacing=0.1)

    # Plot input data
    fig.add_trace(go.Scatter(y=data, mode='lines', name='Input Data'), row=1, col=1)

    # Plot rolling mean and bounds
    fig.add_trace(go.Scatter(y=clipped_data, mode='lines', name='Clipped Data', line=dict(color='red')), row=2, col=1)
    fig.add_trace(go.Scatter(y=rolling_mean, mode='lines', name='Rolling Mean', line=dict(color='gray', dash='dash')), row=2, col=1)
    fig.add_trace(go.Scatter(y=upper_bound, mode='lines', name='Bounds', line=dict(color='orange',  dash='dash')), row=2, col=1)
    fig.add_trace(go.Scatter(y=lower_bound, mode='lines', line=dict(color='orange', dash='dash'),showlegend=False), row=2, col=1)

    # Update layout
    fig.update_layout(
        title=f"Rolling Sigma Clipping, Window Size {window_size}, bounds ({lower_z}, {upper_z})",
        xaxis_title="Index",
        yaxis_title="Value",
        yaxis=dict(title="Data", range=[-5, 5]),
        yaxis2=dict(title="Output", range=[-5, 5]),  
        margin=dict(l=20, r=20, t=80, b=20),
        legend=dict(orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1)
    )

    fig.show()
```

## Implementation Details

### Algorithm

`RollingSigmaClip` operates by maintaining rolling calculations of the mean and standard deviation within a moving window. For each new data point:

1. **Compute Rolling Mean and Standard Deviation**:

     $$ \mu_t = \frac{1}{N} \sum_{i \in W_t} x_i $$

     $$ \sigma_t = \sqrt{\frac{1}{N - 1} \left( \sum_{i \in W_t} x_i^2 - N \mu_t^2 \right)} $$

     Where:
     - $ x_i $ is the $ i $-th data point within the window $ W_t $.
     - $ N $ is the window size.

2. **Calculate Z-Score Bounds**:
   - Determine the dynamic lower and upper bounds based on the rolling mean and standard deviation:

     $$ \text{Lower Bound}_t = \mu_t + (\text{lower} \times \sigma_t) $$

     $$ \text{Upper Bound}_t = \mu_t + (\text{upper} \times \sigma_t) $$

3. **Clip Data Points**:
   - Compare the current data point ($ x_t $) to the calculated bounds:
     - If $ x_t < \text{Lower Bound}_t $, set $ x_t = \text{Lower Bound}_t $.
     - If $ x_t > \text{Upper Bound}_t $, set $ x_t = \text{Upper Bound}_t $.
   - Exclude any clipped data points from future rolling mean and standard deviation calculations to avoid bias.

4. **Apply Correction for Truncation Bias**:

When data is censored by clipping at certain bounds, the observed mean and standard deviation are biased estimates of the true parameters of the underlying normal distribution. To correct for this bias, the following methodology is applied:

4.1. **Standardize Truncation Points**:
   - Compute the standardized truncation points ($ \alpha_t $ and $ \beta_t $) for each window:

     $$ \alpha_t = \frac{a_t - \mu_t}{\sigma_t} $$

     $$ \beta_t = \frac{b_t - \mu_t}{\sigma_t} $$

     Where:
     - $ a_t $ and $ b_t $ are the lower and upper bounds at time $ t $.
     - $ \mu_t $ and $ \sigma_t $ are the observed mean and standard deviation within the window.

4.2. **Compute Correction Factors**:
   - Calculate the probability within the truncation bounds ($ Z_t $):

     $$ Z_t = \Phi(\beta_t) - \Phi(\alpha_t) $$

     - $ \Phi(x) $ is the standard normal cumulative distribution function (CDF).

   - Calculate the adjustment factor for the mean ($ \delta_t $):

     $$ \delta_t = \frac{\phi(\alpha_t) - \phi(\beta_t)}{Z_t} $$

     - $ \phi(x) $ is the standard normal probability density function (PDF).

4.3. **Adjusted Mean**:
   - Correct the observed mean ($ \mu_t $) to estimate the true mean ($ \mu_{\text{true}, t} $):

     $$ \mu_{\text{true}, t} = \mu_t - \sigma_t \delta_t $$

4.4. **Adjusted Variance**:
   - Compute the variance inflation factor ($ V_t $) to adjust the variance:

     $$ V_t = 1 + \frac{\alpha_t \phi(\alpha_t) - \beta_t \phi

(\beta_t)}{Z_t} - \delta_t^2 $$

   - Adjust the observed standard deviation ($ \sigma_t $) to estimate the true standard deviation ($ \sigma_{\text{true}, t} $):

     $$ \sigma_{\text{true}, t} = \sigma_t \times \sqrt{V_t} $$
