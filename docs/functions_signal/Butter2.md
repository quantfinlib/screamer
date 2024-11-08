# `Butter2`

## Description

`Butter2` is a second-order Butterworth low-pass filter designed to smooth a sequence of input data by attenuating high-frequency components. With its inherent characteristics as a maximally flat filter in the passband, it effectively reduces noise while preserving the shape of low-frequency trends. The filter is defined by a specified cutoff frequency, determining the threshold beyond which higher frequencies are attenuated.

The Butterworth filter is computed using a recursive IIR (Infinite Impulse Response) approach, maintaining past input and output values for continuous data processing. This design provides a smooth response without significant ripples in the frequency domain, distinguishing it from other filter types.

### Parameters

**`cutoff_freq`** *(float)*: The normalized cutoff frequency for the low-pass filter, expressed as a fraction of the Nyquist frequency. The range for `cutoff_freq` should be between 0 and 0.5, where 0.5 corresponds to the Nyquist limit.

*NaN handling*: Input containing NaN values may propagate NaNs in the filtered output unless properly pre-processed.

## Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import Butter2

    # Generate synthetic noisy data
    np.random.seed(0)
    data = np.cumsum(np.random.normal(0, 1, 500)) + np.sin(np.linspace(0, 10 * np.pi, 500))

    # Apply the Butterworth filter with a cutoff frequency of 0.1
    butter_filter = Butter2(cutoff_freq=0.1)
    smoothed_data = [butter_filter(d) for d in data]

    # Create subplots with original and smoothed data
    fig = make_subplots(rows=2, cols=1, shared_xaxes=True, row_heights=[1/2, 1/2], vertical_spacing=0.1)

    fig.add_trace(go.Scatter(y=data, mode='lines', name='Original Data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=smoothed_data, mode='lines', name='Smoothed Data', line=dict(color='red')), row=2, col=1)

    fig.update_layout(
        title="Butter2: Second-Order Butterworth Filter",
        xaxis_title="Index",
        yaxis=dict(title="Original Data"),
        yaxis2=dict(title="Smoothed Data"),
        margin=dict(l=20, r=20, t=80, b=20),
        legend=dict(orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1)        
    )

    fig.show()
```

### Formula Details

The `Butter2` class uses the second-order Butterworth difference equation to compute filtered outputs. Given input values $x[i]$ and output values $y[i]$, the recursive relation for the filter output is:

$$
y[i] = b_0 \cdot x[i] + b_1 \cdot x[i-1] + b_2 \cdot x[i-2] - a_1 \cdot y[i-1] - a_2 \cdot y[i-2]
$$

where:
- $b_0, b_1, b_2$ are the feedforward (numerator) coefficients.
- $a_1, a_2$ are the feedback (denominator) coefficients with $a_0$ normalized to 1.

These coefficients are computed based on the cutoff frequency $f_c$ as follows:

1. **Angular pre-warping**:
   
   $$ 
   \omega = \tan(\pi \cdot f_c) 
   $$

2. **Coefficient calculations**:
   - **Denominator**:
  
     $$ 
     \text{denom} = 1 + \sqrt{2} \cdot \omega + \omega^2 
     $$

   - **Feedforward coefficients**:
  
     $$ 
     b_0 = \frac{\omega^2}{\text{denom}}, \quad b_1 = \frac{2 \omega^2}{\text{denom}}, \quad b_2 = \frac{\omega^2}{\text{denom}} 
     $$

   - **Feedback coefficients**:
  
     $$ 
     a_1 = \frac{2(\omega^2 - 1)}{\text{denom}}, \quad a_2 = \frac{1 - \sqrt{2} \cdot \omega + \omega^2}{\text{denom}} 
     $$

These coefficients ensure that the filter maintains a flat response in the passband and smoothly attenuates frequencies beyond the cutoff. The recursive nature of `Butter2` allows for efficient real-time data processing, with each new output computed using the current input and the two preceding input and output values.