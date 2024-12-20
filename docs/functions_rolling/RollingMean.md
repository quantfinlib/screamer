# `RollingMean`

## Description
The `RollingMean` (also known as `moving average`) class computes the mean value within a moving window of specified size over a sequence of data. 

*Parameters*: 
- **`window_size`**: Specifies the size of the rolling window.
- **`start_policy`**: Defines how the function handles the initial phase when fewer than `window_size` data points are available. This parameter accepts one of the following three values:
  - `"strict"`: Returns `NaN` for all calculations until `window_size` elements have been processed.
  - `"expanding"`: Adapts the computation by dynamically reducing the window size to include all available data, starting from a single point and growing until `window_size` is reached.
  - `"zero"`: Simulates a full initial window of zeros, effectively pre-filling the data stream with `window_size` zeros before processing the actual input.
  
## Usage Example and Plot
Below is an example of using `RollingMean` to calculate the rolling mean for a random dataset, along with a plot illustrating its output.

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from screamer import RollingMean

    # Generate example data
    N = 300
    window_size = 30
    data = np.cumsum(np.random.normal(size=300))


    # Plotting with Plotly
    fig = go.Figure()
    fig.add_trace(go.Scatter(y=data, mode='lines', name='Input Data'))
    fig.add_trace(go.Scatter(y=RollingMean(10)(data), mode='lines', name='Rolling Mean 10', line=dict(color='red')))
    fig.add_trace(go.Scatter(y=RollingMean(60)(data), mode='lines', name='Rolling Mean 60', line=dict(color='green')))
    fig.update_layout(title=f"Rolling mean with Window Size 10 and 60",
        xaxis_title="Index",
        yaxis_title="Value",
        margin=dict(l=20, r=20, t=80, b=20),
        legend=dict(orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1)                    
    )
    fig.show()
```


## Implementation Details

### Algorithm

`RollingMean` implements a cyclic buffer.

### Complexity

* **Time Complexity**: `O(1))` per new element due to the insertion and deletion operations in the cyclic buffer.
* **Space Complexity**: `O(window_size)`, as only elements within the current window are stored.


### Performance

* Short streams (n=1.000): 450% faster than `Pandas Rolling mean` and 50% faster than a `numpy cumsum` based approach.
* Longer streams (n=1.000.000): 450% faster than `Pandas Rolling mean` and 270% faster than a `numpy cumsum` based approach.
