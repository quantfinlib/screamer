# `Ffill`

## Description

The `Ffill` class performs forward filling on a sequence of data, replacing any `NaN` values with the most recent non-`NaN` value. This approach is commonly used to handle missing values by carrying forward the last valid observation until a new valid value appears in the data.

*Parameters*: `Ffill` takes no parameters; it simply operates over a data sequence and forward fills any `NaN` values encountered.

*NaN handling*: If a `NaN` appears at the start of the data sequence, it will remain as `NaN` because no preceding value exists to carry forward.

## Usage Example and Plot

Below is an example of using `Ffill` to forward-fill `NaN` values in a data sequence, along with a plot showing the original and filled data.

```{eval-rst}
.. plotly::

.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import Ffill

    # Generate example data with NaN values
    data = np.random.normal(size=30)
    data[[3, 6, 10, 15, 21]] = np.nan  # Introduce NaN values

    # Apply forward fill
    ffilled_data = Ffill()(data)

    # Create subplots with specified row heights and shared x-axis
    fig = make_subplots(
        rows=2, cols=1,
        shared_xaxes=True,
        row_heights=[1/2, 1/2],
        vertical_spacing=0.1
    )

    # Add traces for original data and forward-filled data
    fig.add_trace(go.Scatter(y=data, mode='lines+markers', name='Original Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=ffilled_data, mode='lines+markers', name='Forward-Filled Data', line=dict(color='red')), row=2, col=1)
    
    # Update layout with individual y-axis titles
    fig.update_layout(
        title="Forward Fill (Ffill) on Data with NaNs",
        xaxis_title="Index",
        yaxis_title="Original Data",
        yaxis2_title="Forward-Filled Data"
    )

    fig.show()

```

## Implementation Details

### Algorithm for `Ffill`

The `Ffill` function iterates over the data, replacing each `NaN` value with the most recent valid observation, if available. This ensures that each missing value is filled in a forward direction, but any `NaN` values at the beginning remain unchanged due to the absence of a prior value.

### Complexity


* **Time Complexity**: O(1).
* **Space Complexity**: O(1).

### Performance

 `Ffill` is a lightweight operation that process data efficiently. They are suitable for real-time or streaming data applications where missing values need to be managed with minimal overhead.
