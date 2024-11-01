# `FillNa`

## Description

The `FillNa` class replaces any `NaN` values in a data sequence with a specified fill value. This function is useful for handling missing values by substituting them with a constant, such as 0 or a mean value, which can improve the continuity of data for certain analyses.

*Parameters*:
- **`fill`**: The value to replace `NaN` entries with. This can be any numeric value, allowing customization to fit the context of the data.

*NaN handling*: All `NaN` values are replaced with the specified `fill` value, ensuring no `NaN` values remain in the output data.

## Usage Example and Plot

Below is an example of using `FillNa` to replace `NaN` values with a specified fill value of 0, along with a plot illustrating the effect.

```{eval-rst}
.. plotly::
    :include-source: True

     import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import FillNa

    # Generate example data with NaN values
    data = np.random.normal(size=30)
    data[[3, 6, 10, 15, 21]] = np.nan  # Introduce NaN values

    # Apply forward fill
    filled_data = FillNa(0)(data)

    # Create subplots with specified row heights and shared x-axis
    fig = make_subplots(
        rows=2, cols=1,
        shared_xaxes=True,
        row_heights=[1/2, 1/2],
        vertical_spacing=0.1
    )

    # Add traces for original data and filled data
    fig.add_trace(go.Scatter(y=data, mode='lines+markers', name='Original Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=filled_data, mode='lines+markers', name='Filled Data', line=dict(color='red')), row=2, col=1)
    
    # Update layout with individual y-axis titles
    fig.update_layout(
        title="FillNa(0.0) on Data with NaNs",
        xaxis_title="Index",
        yaxis_title="Original Data",
        yaxis2_title="Filled Data"
    )

    fig.show()
```

---

## Implementation Details

### Algorithm for `FillNa`

The `FillNa` function checks each data point for `NaN` values and substitutes any found with the specified `fill` value, providing a straightforward and efficient method to eliminate `NaN`s from the data.

### Complexity


* **Time Complexity**: O(1)
* **Space Complexity**: O(1)


### Performance

 `Ffill` is a lightweight operation that process data efficiently. They are suitable for real-time or streaming data applications where missing values need to be managed with minimal overhead.
