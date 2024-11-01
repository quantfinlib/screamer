# `RollingMax`

## Description
The `RollingMax` class computes the maximum value within a moving window of specified size over a sequence of data. 


*Initial values*: The constructor requires a positive integer `window_size` parameter to define the rolling window.  
*NaN handling*: NaN values are not handled natively and should be preprocessed if necessary.

## Usage Example and Plot
Below is an example of using `RollingMax` to calculate the rolling maximum for a random dataset, along with a plot illustrating its output.

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from screamer import RollingMax

    # Generate example data
    N = 300
    window_size = 30
    data = np.cumsum(np.random.normal(size=300))

    # Apply rolling maximum calculation
    rolling_max = RollingMax(window_size)
    results = rolling_max(data)

    # Plotting with Plotly
    fig = go.Figure()
    fig.add_trace(go.Scatter(y=data, mode='lines', name='Input Data'))
    fig.add_trace(go.Scatter(y=results, mode='lines', name='Rolling Max', line=dict(color='red')))
    fig.update_layout(
        title=f"Rolling Maximum with Window Size = {window_size}",
        xaxis_title="Index",
        yaxis_title="Value",
        margin=dict(l=20, r=20, t=80, b=20),
        legend=dict(orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1)        
    )
    fig.show()
```


## Implementation Details

### ALgorithm

`RollingMax` used the `ascending minima algorithm` using a deque-based that ensures that each new maximum is calculated in `O(1)` constant time while using `O(window_size)` memory. 

### Complexity:

* Time complexity: `O(1)`
* Space complexity: `O(window_size)`

### Performance

* Short streams (n=1.000): 300% faster than `Pandas Rolling max`
* Longer streams (n=1.000.000): 90% faster than `Pandas Rolling max`
