# `Butter4`

## Description

`Butter4` is a fourth-order Butterworth low-pass filter designed to smooth input data by attenuating high-frequency components while preserving the integrity of lower-frequency signals. As a maximally flat magnitude filter in the passband, it ensures no ripples in the frequency response, making it ideal for applications requiring smooth signal processing without distortion of the original signal's low-frequency content.

The filter is characterized by a specified normalized cutoff frequency, which determines the threshold beyond which higher frequencies are progressively attenuated. `Butter4` implements a recursive Infinite Impulse Response (IIR) filter, utilizing past input and output values to compute the current output, thus efficiently processing real-time data streams.

### Parameters

- **`cutoff_freq`** *(float)*: The normalized cutoff frequency for the low-pass filter, expressed as a fraction of the Nyquist frequency (half the sampling rate). The valid range for `cutoff_freq` is between 0 and 0.5.

*Note on NaN handling*: Inputs containing NaN values may result in NaNs propagating through the filtered output unless appropriately handled before filtering.

## Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import Butter4

    # Generate synthetic noisy data
    np.random.seed(0)
    t = np.linspace(0, 10, 1000)
    data = np.sin(2 * np.pi * 0.5 * t) + 0.5 * np.random.normal(size=1000)

    # Apply the Butterworth filter with a cutoff frequency of 0.1
    butter_filter = Butter4(cutoff_freq=0.1)
    smoothed_data = [butter_filter(d) for d in data]

    # Create subplots with original and smoothed data
    fig = make_subplots(rows=2, cols=1, shared_xaxes=True, vertical_spacing=0.1)

    fig.add_trace(go.Scatter(x=t, y=data, mode='lines', name='Original Data'), row=1, col=1)
    fig.add_trace(go.Scatter(x=t, y=smoothed_data, mode='lines', name='Smoothed Data', line=dict(color='red')), row=2, col=1)

    fig.update_layout(
        title="Butter4: Fourth-Order Butterworth Filter",
        xaxis_title="Time",
        yaxis_title="Amplitude",
        legend=dict(orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1),
        height=600
    )

    fig.show()
```

## Formula Details

### Computation of Filter Coefficients

The `Butter4` filter computes its coefficients based on the digital signal processing principles of the Butterworth filter design. The steps involved in calculating the feedforward (`b`) and feedback (`a`) coefficients for a fourth-order Butterworth low-pass filter are as follows:

#### 1. Pre-warping the Digital Cutoff Frequency

To compensate for the frequency distortion introduced by the bilinear transform, the normalized digital cutoff frequency \( f_c \) is pre-warped to the analog domain using:

$$
\omega_c = 2 \cdot \tan\left( \frac{\pi f_c}{2} \right)
$$

#### 2. Calculating the Analog Filter Poles

For a fourth-order filter (\( N = 4 \)), the poles of the analog Butterworth filter are located uniformly around the left half of the complex \( s \)-plane unit circle. The pole locations \( s_k \) are calculated using:

$$
s_k = \omega_c \cdot e^{j \theta_k}
$$

where \( \theta_k \) is:

$$
\theta_k = \frac{\pi}{2} + \frac{(2k - 1)\pi}{2N}, \quad k = 1, 2, 3, 4
$$

#### 3. Mapping Analog Poles to Digital Poles using the Bilinear Transform

The analog poles \( s_k \) are transformed to digital poles \( z_k \) using the bilinear transform:

$$
z_k = \frac{1 + \frac{s_k}{2}}{1 - \frac{s_k}{2}}
$$

#### 4. Formulating the Denominator Polynomial \( A(z) \)

The digital filter's denominator polynomial \( A(z) \) is constructed from the digital poles:

$$
A(z) = (1 - z_1^{-1})(1 - z_2^{-1})(1 - z_3^{-1})(1 - z_4^{-1})
$$

For computational purposes, this involves expanding the product of terms corresponding to each pole (taking complex conjugate pairs together to ensure real coefficients):

$$
A(z) = 1 + a_1 z^{-1} + a_2 z^{-2} + a_3 z^{-3} + a_4 z^{-4}
$$

#### 5. Formulating the Numerator Polynomial \( B(z) \)

For a low-pass Butterworth filter, all zeros are at \( z = -1 \). The numerator polynomial \( B(z) \) is given by:

$$
B(z) = (1 + z^{-1})^N
$$

Expanding for \( N = 4 \):

$$
B(z) = 1 + 4 z^{-1} + 6 z^{-2} + 4 z^{-3} + z^{-4}
$$

#### 6. Normalizing the Filter Coefficients

To ensure unity gain at zero frequency (DC gain), the filter coefficients are normalized. The normalization factor \( K \) is calculated as:

$$
K = \frac{\sum_{n=0}^{N} a_n}{\sum_{n=0}^{N} b_n}
$$

The normalized feedforward coefficients are then:

$$
b_n = K \cdot b_n, \quad n = 0, 1, 2, 3, 4
$$

### Summary of the Coefficients

After performing the above calculations, the filter coefficients \( a_n \) and \( b_n \) are determined, resulting in the difference equation:

$$
y[i] = b_0 x[i] + b_1 x[i-1] + b_2 x[i-2] + b_3 x[i-3] + b_4 x[i-4] - a_1 y[i-1] - a_2 y[i-2] - a_3 y[i-3] - a_4 y[i-4]
$$

### Application of the Filter using `lfilter`

The filter operates recursively, applying the calculated coefficients to the input data sequence. The implementation resembles the standard Direct Form II transposed structure of an IIR filter.

#### Difference Equation

The output \( y[i] \) at each discrete time \( i \) is computed as:

$$
\begin{align*}
v[i] &= x[i] - a_1 v[i-1] - a_2 v[i-2] - a_3 v[i-3] - a_4 v[i-4] \\
y[i] &= b_0 v[i] + b_1 v[i-1] + b_2 v[i-2] + b_3 v[i-3] + b_4 v[i-4]
\end{align*}
$$

Alternatively, the combined difference equation is:

$$
y[i] = \sum_{k=0}^{4} b_k x[i - k] - \sum_{k=1}^{4} a_k y[i - k]
$$

#### Initialization

The initial conditions \( y[i], x[i] \) for \( i < 0 \) are typically set to zero unless specified otherwise.

#### Stability and Causality

The filter is designed to be stable and causal, ensuring that the output at any time \( i \) depends only on current and past input and output values.

### Notes on Implementation

- **Zero-Padding**: For real-time data, appropriate handling of the initial transient response is necessary, which may involve zero-padding or applying initial conditions.

- **Precision**: Due to recursive calculations, using double-precision floating-point arithmetic helps minimize numerical errors.

- **Frequency Response**: The Butterworth filter provides a smooth roll-off around the cutoff frequency, with a magnitude response that is maximally flat in the passband.

By following these calculations and applying the coefficients using the recursive difference equation, the `Butter4` filter effectively attenuates high-frequency components, providing a smooth and reliable low-pass filtering of the input data.