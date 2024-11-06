# `Butter`

## Description

`Butter` is a generic-order Butterworth low-pass filter. This filter design ensures a maximally flat frequency response in the passband, ideal for applications that require minimal ripple while attenuating high-frequency components beyond a specified cutoff frequency. The order of the filter, specified as `N`, determines the sharpness of the frequency roll-off, with higher orders providing steeper transitions. This low-pass filter is particularly useful for smoothing data, reducing noise, and maintaining the signal's essential low-frequency components.

The Butterworth filter is implemented using a digital Infinite Impulse Response (IIR) filter design, which converts the analog filter specifications to the digital domain. This design leverages the bilinear transform to map the continuous filter’s poles and zeros into the discrete domain.

### Parameters

**`N`** *(int)*: The order of the filter, specifying the steepness of the frequency cutoff. A higher order results in a sharper cutoff but may introduce more computational complexity.

**`cutoff_freq`** *(float)*: The normalized cutoff frequency for the low-pass filter, expressed as a fraction of the Nyquist frequency (half the sampling rate). It must be in the range 0 to 0.5.

*NaN handling*: NaN values may propagate through the filter unless handled separately in preprocessing.

## Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import Butter

    # Generate synthetic noisy data
    np.random.seed(0)
    data = np.cumsum(np.random.normal(0, 1, 500)) + np.sin(np.linspace(0, 10 * np.pi, 500))


    # Create subplots with original and smoothed data
    fig = make_subplots(rows=3, cols=1, shared_xaxes=True, row_heights=[1/3, 1/3, 1/3], vertical_spacing=0.1)

    fig.add_trace(go.Scatter(y=data, mode='lines', name='Original Data'), row=1, col=1)

    fig.add_trace(go.Scatter(
      y=Butter(order=4, cutoff_freq=0.4)(data), 
      mode='lines', name='order=4, qf=0.4', line=dict(color='purple')), row=2, col=1)   
    fig.add_trace(go.Scatter(
      y=Butter(order=4, cutoff_freq=0.1)(data), 
      mode='lines', name='order=4, qf=0.1', line=dict(color='red')), row=2, col=1)
    fig.add_trace(go.Scatter(
      y=Butter(order=4, cutoff_freq=0.05)(data), 
      mode='lines', name='order=4, qf=0.05', line=dict(color='orange')), row=2, col=1)

    fig.add_trace(go.Scatter(
      y=Butter(order=2, cutoff_freq=0.1)(data), 
      mode='lines', name='order=2, qf=0.1', line=dict(color='blue')), row=3, col=1)
    fig.add_trace(go.Scatter(
      y=Butter(order=8, cutoff_freq=0.1)(data), 
      mode='lines', name='order=8, qf=0.1', line=dict(color='green')), row=3, col=1)


    fig.update_layout(
        title="Butterworth Low-Pass Filters",
        xaxis_title="Index",
        yaxis=dict(title="Original Data"),
        yaxis2=dict(title="Cutoff freq variants"),
        yaxis3=dict(title="Order variants"),
        margin=dict(l=20, r=20, t=120, b=20),
        legend=dict(orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1)        
    )

    fig.show()
```

### Formula Details

The `Butter` class employs a series of steps to compute the filter's coefficients and apply the low-pass transformation. Given the filter order \( N \) and cutoff frequency \( f_c \), the process involves:

1. **Pole Calculation**: The poles for an analog Butterworth filter are distributed evenly on the left half of the complex plane unit circle. For each pole \( p_k \), we calculate:

   $$
   p_k = -\exp\left(\frac{i \pi (2k - 1)}{2N}\right) \quad \text{for} \; k = 1, \dots, N
   $$

2. **Frequency Warping**: To prepare the analog filter for digital conversion, we pre-warp the cutoff frequency to adjust for the bilinear transformation's non-linear frequency mapping. The warped angular frequency \( \omega \) is:

   $$
   \omega = 2 f_s \tan\left(\frac{\pi f_c}{f_s}\right)
   $$

   where \( f_s \) is the sampling frequency, typically set to 2 for normalized frequencies.

3. **Low-Pass Transformation**: Each pole is scaled by the cutoff frequency \( \omega \) to implement the low-pass response:

   $$
   p_k = \omega \cdot p_k
   $$

4. **Bilinear Transform**: The bilinear transform maps each pole \( p_k \) and any zero \( z_k \) (if present) from the analog domain to the digital z-plane, ensuring stability and frequency response fidelity. For each transformed pole \( p_z \), we apply:

   $$
   p_z = \frac{2 f_s + p_k}{2 f_s - p_k}
   $$

   Any zeros at infinity are placed at the Nyquist frequency (i.e., \( -1 \) in the z-plane).

5. **Gain Compensation**: To ensure the filter's gain remains consistent, we compensate for any changes caused by the bilinear transform. The gain \( K \) in the z-domain is adjusted as:

   $$
   K_z = K \cdot \frac{\prod_{k} (2 f_s - z_k)}{\prod_{k} (2 f_s - p_k)}
   $$

6. **Polynomial Conversion**: The poles and zeros in the z-domain are converted to the transfer function form \( H(z) = \frac{B(z)}{A(z)} \), with numerator coefficients \( b \) and denominator coefficients \( a \) computed from the polynomial expansion of the zeros and poles:

   - **Numerator Polynomial** (from zeros):
     $$
     B(z) = K_z \prod_{k} (z - z_k)
     $$
   - **Denominator Polynomial** (from poles):
     $$
     A(z) = \prod_{k} (z - p_k)
     $$

These recursive IIR filter coefficients are then applied to process the input data, recursively updating past inputs and outputs to generate the filtered signal. The Butterworth design’s unique response shape ensures a smooth roll-off at the specified cutoff frequency with no ripple in the passband, making it ideal for applications requiring a clean low-pass filter response.