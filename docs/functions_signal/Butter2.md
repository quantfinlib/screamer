# `Butter2`

## Description

`Butter2` implements a 2nd-degree Butterworth lowpass filter to attenuate frequencies above a specified cutoff while preserving lower frequencies. This type of filter is known for its maximally flat response in the passband, ensuring minimal distortion within the desired frequency range.

*Equation*:

The transfer function for a 2nd-degree Butterworth filter in the z-domain is given by:

$$
H(z) = \frac{b_0 + b_1 z^{-1} + b_2 z^{-2}}{1 + a_1 z^{-1} + a_2 z^{-2}}
$$

where the coefficients \(b_0, b_1, b_2, a_1,\) and \(a_2\) are derived based on the chosen cutoff frequency.

*Parameters*:

- `cutoff_freq` (double): The normalized cutoff frequency, \(0 < \text{cutoff\_freq} < \frac{1}{2}\), corresponding to Nyquist sampling theory.

*NaN handling*: Input `NaN` values propagate through the filter, maintaining alignment in the output.

## Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from scipy.signal import butter, filtfilt
    from screamer import Butter2

    # Generate sample data with noise
    np.random.seed(0)
    t = np.linspace(0, 1, 500)
    data = np.sin(2 * np.pi * 5 * t) + 0.5 * np.random.normal(size=t.shape)

    # Apply Butterworth filter
    butter_filtered = Butter2(0.1)(data)

    fig = make_subplots(rows=2, cols=1, shared_xaxes=True, row_heights=[1/2, 1/2], vertical_spacing=0.1)

    fig.add_trace(go.Scatter(y=data, mode='lines', name='Original Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=butter_filtered, mode='lines', name='Filtered Data (Cutoff 0.1)', line=dict(color='orange')), row=2, col=1)

    fig.update_layout(
        title="2nd-Degree Butterworth Lowpass Filter",
        xaxis_title="Time (s)",
        yaxis=dict(title="Original Data"),
        yaxis2=dict(title="Filtered Data"),
        margin=dict(l=20, r=20, t=60, b=20),
        legend=dict(orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1)
    )

    fig.show()
```

## Implementation Details

`Butter2` applies a 2nd-degree Butterworth filter using recursive digital filtering techniques. The filter coefficients are determined based on the given `cutoff_freq`, which must adhere to Nyquist constraints to avoid aliasing. Internally, the implementation ensures stability and efficiency, employing forward and backward filtering to prevent phase distortion. The coefficients \(b_0, b_1, b_2, a_1,\) and \(a_2\) are calculated using the bilinear transform for digital filter design.

