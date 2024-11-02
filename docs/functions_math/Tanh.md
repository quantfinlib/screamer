# `Tanh`

## Description

The `Tanh` class applies the hyperbolic tangent function, which maps any real number into the range \(-1\) to \(1\). It is often used in neural networks as an activation function due to its zero-centered nature.

*Equation*:

$$
f(x) = \tanh(x) = \frac{e^x - e^{-x}}{e^x + e^{-x}}
$$

*Parameters*: `Tanh` takes no parameters.

*NaN handling*: `NaN` values are not modified by this function.

## Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from screamer import Tanh

    # Generate example data
    data = np.linspace(-3, 3, 100)
    tanh_data = Tanh()(data)

    fig = go.Figure()
    fig.add_trace(go.Scatter(y=data, mode='lines', name='Original Data'))
    fig.add_trace(go.Scatter(y=tanh_data, mode='lines', name='Tanh Output', line=dict(color='blue')))

    fig.update_layout(
        title="Tanh Transformation",
        yaxis_title="Output",
        xaxis_title="Input",
        margin=dict(l=20, r=20, t=40, b=20)
    )

    fig.show()
```