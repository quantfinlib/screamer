# `Softsign`

## Description

The `Softsign` class smooths input values into the range \(-1\) to \(1\) with asymptotic behavior, providing a continuous, differentiable transformation useful for activation functions.

*Equation*:

$$
f(x) = \frac{x}{1 + |x|}
$$

*Parameters*: `Softsign` takes no parameters.

*NaN handling*: `NaN` values are not modified by this function.

## Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from screamer import Softsign

    # Generate example data
    data = np.linspace(-5, 5, 100)
    softsign_data = Softsign()(data)

    fig = go.Figure()
    fig.add_trace(go.Scatter(y=data, mode='lines', name='Original Data'))
    fig.add_trace(go.Scatter(y=softsign_data, mode='lines', name='Softsign Output', line=dict(color='brown')))

    fig.update_layout(
        title="Softsign Transformation",
        yaxis_title="Output",
        xaxis_title="Input",
        margin=dict(l=20, r=20, t=40, b=20)
    )

    fig.show()
```