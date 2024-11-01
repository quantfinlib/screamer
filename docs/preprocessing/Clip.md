# `Clip`

## Description

The `Clip` class restricts each value in a data sequence to fall within a specified range. Values outside the defined lower and upper bounds are clipped to the nearest boundary, ensuring that the processed data stays within the desired range. This function is useful in scenarios where extreme values or outliers need to be managed or excluded from further analysis.

*Parameters*: 
- **`lower`** (optional): The minimum allowable value. If a data point is below this threshold, it will be set to `lower`. If unspecified, there is no lower bound.
- **`upper`** (optional): The maximum allowable value. If a data point exceeds this threshold, it will be set to `upper`. If unspecified, there is no upper bound.

*NaN handling*: NaN values are not modified by this function and remain as NaN if present in the input data.

## Usage Example and Plot

Below is an example of using `Clip` to constrain data to fall between -1 and 1, along with a plot illustrating the effect.

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from screamer import Clip

    # Generate example data
    data = np.random.normal(size=50)

    # Clip data to fall between -1 and 1
    clipped_data = Clip(lower=-1, upper=1)(data)

    # Create plot with input data and clipped data
    fig = go.Figure()
    fig.add_trace(go.Scatter(y=data, mode='lines+markers', name='Input Data'))
    fig.add_trace(go.Scatter(y=clipped_data, mode='lines+markers', name='Clipped Data', line=dict(color='red')))
    
    fig.update_layout(
        title="Data Clipping with Bounds (-1, 1)",
        xaxis_title="Index",
        yaxis_title="Value",
        yaxis=dict(range=[-2, 2])  # Limit y-axis to show the clipping effect clearly
    )

    fig.show()
```

## Implementation Details

### Algorithm

The `Clip` function performs an element-wise check on each value in the input data:
- If a lower bound (`lower`) is specified, any value below this threshold is replaced with `lower`.
- If an upper bound (`upper`) is specified, any value exceeding this threshold is replaced with `upper`.

This element-wise operation ensures that each data point falls within the specified range, with minimal computational overhead.

### Complexity

* **Time Complexity**: `O(1)`.
* **Space Complexity**: `O(1)`.

### Performance

The `Clip` has comparable speed to  numpy's clip, and is approximately 10x faster than Pandas clip..