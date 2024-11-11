# `RollingPoly2`

## Description

The `RollingPoly2` class fits a quadratic polynomial (second-degree equation) to the data within a specified moving window. This rolling quadratic fit provides insights into the data’s overall trend, rate of change, and curvature. Depending on the `derivative_order`, the function can return:

- **0**: The y-value at the endpoint of the fitted quadratic curve, offering a smooth continuation of the data.
- **1**: The slope (first derivative) at the endpoint, representing the rate of change.
- **2**: The curvature (second derivative) at the endpoint, which captures the concavity of the data.

This method, which extends the idea of a causal Savitzky-Golay filter, enables more nuanced data analysis by capturing both linear and quadratic trends within each window.

*Parameters*: 
- **`window_size`**: Specifies the size of the rolling window.
- **`derivative_order`**: 
  - `0`: The y-value at the endpoint of the fitted quadratic curve, offering a smooth continuation of the data.
  - `1`: The slope (first derivative) at the endpoint, representing the rate of change.
  - `2`: The curvature (second derivative) at the endpoint, which captures the concavity of the data.
- **`start_policy`**: Defines how the function handles the initial phase when fewer than `window_size` data points are available. This parameter accepts one of the following three values:
  - `"strict"`: Returns `NaN` for all calculations until `window_size` elements have been processed.
  - `"expanding"`: Adapts the computation by dynamically reducing the window size to include all available data, starting from a single point and growing until `window_size` is reached.
  - `"zero"`: Simulates a full initial window of zeros, effectively pre-filling the data stream with `window_size` zeros before processing the actual input.

## Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import RollingPoly2

    # Generate example data
    data = np.cumsum(np.random.normal(size=300))

    # Create subplots with specified row heights and shared x-axis
    fig = make_subplots(
        rows=3, cols=1,
        shared_xaxes=True,
        row_heights=[1/3, 1/3, 1/3],
        vertical_spacing=0.1
    )

    # RollingPoly2 for endpoint (derivative_order=0)
    endpoint_data = RollingPoly2(window_size=30, derivative_order=0)(data)

    # RollingPoly2 for slope (derivative_order=1)
    slope_data = RollingPoly2(window_size=30, derivative_order=1)(data)

    # RollingPoly2 for curvature (derivative_order=2)
    curvature_data = RollingPoly2(window_size=30, derivative_order=2)(data)

    # Add traces for input data and results with different derivative orders
    fig.add_trace(go.Scatter(y=data, mode='lines', name='Input Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=endpoint_data, mode='lines', name='Rolling Endpoint (Order 0)', line=dict(color='green')), row=1, col=1)
    fig.add_trace(go.Scatter(y=slope_data, mode='lines', name='Rolling Slope (Order 1)', line=dict(color='blue')), row=2, col=1)
    fig.add_trace(go.Scatter(y=curvature_data, mode='lines', name='Rolling Curvature (Order 2)', line=dict(color='purple')), row=3, col=1)

    # Update layout with titles and axis labels
    fig.update_layout(
        title="RollingPoly2 with Window Size 30",
        xaxis_title="Index",
        yaxis=dict(title="Input Data and endpoint"),
        yaxis2=dict(title="RollingPoly2 Slope"),
        yaxis3=dict(title="RollingPoly2 Curvature"),
        margin=dict(l=20, r=20, t=120, b=20),
        legend=dict(orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1)        
    )

    fig.show()
```

---

## Implementation Details

### Algorithm

`RollingPoly2` uses least-squares regression to fit a quadratic function within each window. The class allows returning either the endpoint value, the slope, or the curvature at the endpoint, based on `derivative_order`. This fitting method effectively captures both linear and non-linear trends, making it suitable for applications needing insights into the rate of change and concavity within the data.

### Complexity

* **Time Complexity**: O(1) per element.
* **Space Complexity**: O(window_size), since only elements within the current window are stored.

### Performance

* `RollingPoly2` is hundreds times faster than `Pandas rolling apply`, and the speed is constant, indepenent of the windows size.