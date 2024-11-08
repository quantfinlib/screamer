# `Sigmoid`

## Description

The `Sigmoid` class transforms input values into the range \(0\) to \(1\), making it a staple activation function in neural networks, especially for binary classification problems.

*Equation*:

$$
f(x) = \frac{1}{1 + e^{-x}}
$$

*Parameters*: `Sigmoid` takes no parameters.

*NaN handling*: `NaN` values are not modified by this function.

## Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from screamer import Sigmoid

    # Generate example data
    data = np.linspace(-6, 6, 100)
    sigmoid_data = Sigmoid()(data)

    fig = go.Figure()
    fig.add_trace(go.Scatter(y=data, mode='lines', name='Original Data'))
    fig.add_trace(go.Scatter(y=sigmoid_data, mode='lines', name='Sigmoid Output', line=dict(color='red')))

    fig.update_layout(
        title="Sigmoid Transformation",
        yaxis_title="Output",
        xaxis_title="Input",
        margin=dict(l=20, r=20, t=40, b=20)
    )

    fig.show()
```