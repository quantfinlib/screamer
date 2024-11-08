# `Selu`

## Description

The `Selu` (Scaled Exponential Linear Unit) class scales the ELU function to induce self-normalizing properties, which help stabilize neural network training by keeping activations within a desired range.

*Equation*:

$$
f(x) = \lambda \times \begin{cases} 
x, & \text{if } x > 0 \\
\alpha (e^x - 1), & \text{if } x \leq 0
\end{cases}
$$

where $\lambda \approx 1.0507$ and $\alpha \approx 1.67326$.

*Parameters*: No parameters.

*NaN handling*: `NaN` values are not modified by this function.

## Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from screamer import Selu

    # Generate example data
    data = np.linspace(-3, 3, 100)
    selu_data = Selu()(data)

    fig = go.Figure()
    fig.add_trace(go.Scatter(y=data, mode='lines', name='Original Data'))
    fig.add_trace(go.Scatter(y=selu_data, mode='lines', name='SELU Output', line=dict(color='orange')))

    fig.update_layout(
        title="Selu Transformation",
        yaxis_title="Output",
        xaxis_title="Input",
        margin=dict(l=20, r=20, t=40, b=20)
    )

    fig.show()
```