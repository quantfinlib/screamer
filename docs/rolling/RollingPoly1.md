# `RollingPoly1`

## Description

The `RollingPoly1` class fits a straight line (first-degree polynomial) through data within a specified moving window. It returns either the endpoint of this line or the slope, depending on the `derivative_order` argument. This approach, also known as a **causal Savitzky-Golay filter**, provides a way to smooth data or obtain the rate of change across each window. With `derivative_order=0`, the function returns the y-value at the end of the fitted line, which smooths the data over the window. When `derivative_order=1`, it outputs the slope, capturing the rate of change within each window.

*Initial values*: The constructor requires a positive integer `window_size` to define the rolling window and an integer `derivative_order` (0 or 1) to specify the type of output.  
*NaN handling*: NaN values are not handled natively and should be preprocessed if necessary.

## Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import RollingPoly1

    # Generate example data
    data = np.cumsum(np.random.normal(size=300))

    # Create subplots with specified row heights and shared x-axis
    fig = make_subplots(
        rows=2, cols=1,
        shared_xaxes=True,
        row_heights=[2/3, 1/3],
        vertical_spacing=0.1
    )

    # RollingPoly1 for endpoint (derivative_order=0)
    endpoint_data = RollingPoly1(window_size=30, derivative_order=0)(data)

    # RollingPoly1 for slope (derivative_order=1)
    slope_data = RollingPoly1(window_size=30, derivative_order=1)(data)

    # Add traces for input data and results with different derivative orders
    fig.add_trace(go.Scatter(y=data, mode='lines', name='Input Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=endpoint_data, mode='lines', name='Rolling Endpoint (Order 0)', line=dict(color='red')), row=1, col=1)
    fig.add_trace(go.Scatter(y=slope_data, mode='lines', name='Rolling Slope (Order 1)', line=dict(color='green')), row=2, col=1)

    # Update layout with titles and axis labels
    fig.update_layout(
        title="RollingPoly1 with Window Size 30",
        xaxis_title="Index",
        yaxis=dict(title="Input Data"),
        yaxis2=dict(title="RollingPoly1 Output")
    )

    fig.show()
```

---

## Implementation Details

### Algorithm

`RollingPoly1` uses a cyclic buffers to implement an online variant of least-squares regression to fit a straight line through data in the specified window. Depending on `derivative_order`, it either outputs the endpoint of this line (0th derivative, for smoothing) or the slope (1st derivative, for change rate). This is efficient and suitable for streaming data applications where real-time smoothing or trend analysis is required.

### Complexity

* **Time Complexity**: O(1) per element.
* **Space Complexity**: O(window_size), since only elements within the current window are stored.

### Performance

* `RollingPoly1` is hundreds times faster than `Pandas rolling apply`, and the speed is constant, indepenent of the windows size.
