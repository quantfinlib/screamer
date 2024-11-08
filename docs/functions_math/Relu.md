# `Relu`

## Description

The `Relu` class implements the Rectified Linear Unit activation function, a common function in neural networks and data processing that outputs the input directly if it is positive and outputs zero otherwise. This function is especially useful for introducing non-linearity while maintaining positive gradients.

*Equation*:

$$
f(x) = \max(0, x)
$$

*Parameters*: `Relu` takes no parameters.

*NaN handling*: `NaN` values are not modified by this function.

## Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from screamer import Relu

    # Generate example data with negative and positive values
    data = np.linspace(-3, 3, 100)
    relu_data = Relu()(data)

    fig = go.Figure()
    fig.add_trace(go.Scatter(y=data, mode='lines', name='Original Data'))
    fig.add_trace(go.Scatter(y=relu_data, mode='lines', name='ReLU Output', line=dict(color='green')))

    fig.update_layout(
        title="Relu Transformation",
        yaxis_title="Output",
        xaxis_title="Input",
        margin=dict(l=20, r=20, t=40, b=20)
    )

    fig.show()
```
