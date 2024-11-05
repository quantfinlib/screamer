# `ButterN`

## Description

`ButterN` is a general-purpose Butterworth low-pass filter of order \( N \), designed to attenuate frequencies above a specified cutoff frequency while preserving the characteristics of lower-frequency signals. The Butterworth filter is known for its maximally flat magnitude response in the passband, ensuring a smooth and ripple-free transition.

This filter computes the coefficients for a digital Infinite Impulse Response (IIR) filter using the bilinear transformation method, mapping an analog Butterworth filter design to the digital domain. The implementation supports filters of any positive integer order \( N \), allowing flexibility in designing filters with varying steepness in the frequency response.

### Parameters

- **`N`** *(int)*: The order of the Butterworth filter. Must be a positive integer.
- **`cutoff_freq`** *(float)*: The normalized cutoff frequency for the low-pass filter, specified as a fraction between 0 and 1, where 1 corresponds to the Nyquist frequency (half the sampling rate).

*Note on NaN handling*: Inputs containing NaN values may result in NaNs propagating through the filtered output unless appropriately handled before filtering.

## Formula Details

### Overview

The `ButterN` filter computes its coefficients based on the analog prototype of the Butterworth filter and then applies the bilinear transform to convert it into a digital filter. The key steps involved are:

1. **Pre-warping the digital cutoff frequency to the analog domain**.
2. **Calculating the analog filter poles**.
3. **Mapping the analog poles to the digital domain using the bilinear transform**.
4. **Forming the denominator polynomial from the digital poles**.
5. **Forming the numerator polynomial for a low-pass filter**.
6. **Normalizing the filter coefficients to ensure unity gain at zero frequency**.

### Detailed Calculations

#### 1. Pre-warping the Digital Cutoff Frequency

To compensate for the frequency distortion introduced by the bilinear transform, the normalized digital cutoff frequency \( f_c \) is pre-warped to the analog (continuous-time) domain using:

$$
\omega_c = \tan\left( \pi f_c \right)
$$

#### 2. Calculating the Analog Filter Poles

The poles of the analog \( N \)-th order Butterworth filter are calculated using:

$$
s_k = \omega_c \cdot e^{j\theta_k}
$$

where:

- \( k = 1, 2, \dots, N \)
- \( \theta_k = \frac{\pi}{2} + \frac{(2k - 1)\pi}{2N} \)

This positions the poles uniformly around the left half of the complex \( s \)-plane unit circle.

#### 3. Mapping the Analog Poles to the Digital Domain

The analog poles \( s_k \) are mapped to the digital domain \( z_k \) using the bilinear transform:

$$
z_k = \frac{1 + s_k}{1 - s_k}
$$

#### 4. Forming the Denominator Polynomial \( A(z) \)

The denominator polynomial is constructed from the digital poles:

- For each pair of complex conjugate poles \( z_k \) and \( z_k^* \), form a quadratic factor:

  $$
  A_k(z) = 1 - 2 \text{Re}(z_k) z^{-1} + |z_k|^2 z^{-2}
  $$

- For any real poles, form a linear factor:

  $$
  A_k(z) = 1 - z_k z^{-1}
  $$

- Multiply all the factors together to form the denominator polynomial:

  $$
  A(z) = \prod_{k=1}^{N} A_k(z)
  $$

#### 5. Forming the Numerator Polynomial \( B(z) \)

For a low-pass Butterworth filter, all zeros are at \( z = -1 \). The numerator polynomial is:

$$
B(z) = (1 + z^{-1})^N
$$

This can be expanded using the binomial theorem to obtain the coefficients.

#### 6. Normalizing the Filter Coefficients

To ensure the filter has unity gain at zero frequency (DC), the coefficients are normalized:

- Compute the sum of the numerator coefficients:

  $$
  \sum_{n=0}^{N} b_n = B(1)
  $$

- Compute the sum of the denominator coefficients:

  $$
  \sum_{n=0}^{N} a_n = A(1)
  $$

- Calculate the gain factor:

  $$
  K = \frac{\sum_{n=0}^{N} a_n}{\sum_{n=0}^{N} b_n}
  $$

- Scale the numerator coefficients:

  $$
  b_n = K \cdot b_n, \quad n = 0, 1, \dots, N
  $$

#### 7. Sign Convention for the Difference Equation

The standard form of the digital filter difference equation is:

$$
y[n] = \frac{1}{a_0} \left( \sum_{k=0}^{N} b_k x[n - k] - \sum_{k=1}^{N} a_k y[n - k] \right)
$$

- Note that the \( a_k \) coefficients (for \( k \geq 1 \)) are subtracted in the equation.
- Therefore, the \( a_k \) coefficients beyond \( a_0 \) are stored with positive signs in the coefficients array, and the implementation should reflect this convention.

### Summary of the Coefficients

After performing the above calculations, the filter coefficients \( a_n \) and \( b_n \) are determined, resulting in the difference equation:

$$
y[n] = \frac{1}{a_0} \left( b_0 x[n] + b_1 x[n - 1] + \dots + b_N x[n - N] - a_1 y[n - 1] - a_2 y[n - 2] - \dots - a_N y[n - N] \right)
$$

### Implementation Notes

- **Coefficient Arrays**:

  - The numerator coefficients \( b_n \) are stored in an array `b` of length \( N + 1 \).
  - The denominator coefficients \( a_n \) are stored in an array `a` of length \( N + 1 \), with \( a_0 = 1 \) and subsequent coefficients reflecting the sign convention.

- **Zero-Padding and Initial Conditions**:

  - For real-time data processing, initial conditions should be handled appropriately.
  - Typically, past input and output values \( x[n - k] \) and \( y[n - k] \) for \( k > 0 \) are initialized to zero unless specified otherwise.

- **Stability and Causality**:

  - The filter is designed to be stable and causal, with all poles inside the unit circle in the \( z \)-plane.

- **Precision**:

  - Double-precision floating-point arithmetic is recommended to minimize numerical errors in the coefficient calculations and recursive filtering.

### Example

Suppose we want to design a 4th-order Butterworth low-pass filter with a normalized cutoff frequency of \( f_c = 0.2 \).

**Steps**:

1. **Pre-warp the cutoff frequency**:

   $$
   \omega_c = \tan\left( \pi \times 0.2 \right) = \tan\left( 0.62832 \right) \approx 0.7265
   $$

2. **Calculate the analog poles**:

   For \( N = 4 \), \( k = 1, 2, 3, 4 \):

   - \( \theta_1 = \frac{\pi}{2} + \frac{(2 \times 1 - 1)\pi}{2 \times 4} = 1.9635 \)
   - \( s_1 = 0.7265 \cdot e^{j1.9635} \)

   (Repeat for \( k = 2, 3, 4 \))

3. **Map the poles to the digital domain**:

   \( z_k = \frac{1 + s_k}{1 - s_k} \)

4. **Form the denominator polynomial \( A(z) \)**:

   Multiply the factors corresponding to the poles.

5. **Form the numerator polynomial \( B(z) \)**:

   \( B(z) = (1 + z^{-1})^4 \)

6. **Normalize the coefficients**:

   - Compute sums \( \sum b_n \) and \( \sum a_n \).
   - Calculate gain \( K \) and scale \( b_n \).

7. **Resulting Coefficients**:

   - Numerator coefficients \( b_n \):

     \( [0.00482434, 0.01929737, 0.02894606, 0.01929737, 0.00482434] \)

   - Denominator coefficients \( a_n \):

     \( [1, -2.369513, 2.313988, -1.054665, 0.187379] \)

## Application of the Filter

Once the coefficients are computed, the filter can be applied to a data sequence \( x[n] \) using the difference equation:

$$
y[n] = \frac{1}{a_0} \left( b_0 x[n] + b_1 x[n - 1] + \dots + b_N x[n - N] - a_1 y[n - 1] - a_2 y[n - 2] - \dots - a_N y[n - N] \right)
$$

- At each time step \( n \), the output \( y[n] \) is calculated using the current and previous inputs, and previous outputs.

## Usage Example and Plot

```{eval-rst}
.. plotly::
    :include-source: True

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import ButterN

    # Generate synthetic noisy data
    np.random.seed(0)
    t = np.linspace(0, 10, 1000)
    data = np.sin(2 * np.pi * 0.5 * t) + 0.5 * np.random.normal(size=1000)

    # Apply a 4th-order Butterworth filter with a cutoff frequency of 0.2
    butter_filter = ButterN(N=4, cutoff_freq=0.2)
    smoothed_data = [butter_filter(d) for d in data]

    # Create subplots with original and smoothed data
    fig = make_subplots(rows=2, cols=1, shared_xaxes=True, vertical_spacing=0.1)

    fig.add_trace(go.Scatter(x=t, y=data, mode='lines', name='Original Data'), row=1, col=1)
    fig.add_trace(go.Scatter(x=t, y=smoothed_data, mode='lines', name='Smoothed Data', line=dict(color='red')), row=2, col=1)

    fig.update_layout(
        title="ButterN: N-th Order Butterworth Filter (N=4)",
        xaxis_title="Time",
        yaxis_title="Amplitude",
        legend=dict(orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1),
        height=600
    )

    fig.show()
```

## Notes on Implementation

- **Extensibility**:

  - The `ButterN` filter is designed to handle any positive integer order \( N \), both even and odd.
  - Higher-order filters provide a sharper cutoff but may introduce more phase distortion.

- **Stability Considerations**:

  - For higher-order filters, numerical stability can be a concern.
  - Using cascaded second-order sections (biquad filters) can improve stability.

- **Performance**:

  - The recursive implementation allows efficient real-time filtering.
  - Care should be taken to manage numerical precision, especially for high-order filters.

- **Conventions**:

  - The filter coefficients follow the standard signal processing conventions, ensuring compatibility with common filtering functions and libraries.
  - The sign convention for the \( a_k \) coefficients aligns with the standard difference equation, where they are subtracted.

## Conclusion

The `ButterN` filter provides a flexible and effective means of low-pass filtering data, with the ability to specify the filter order and cutoff frequency according to the needs of the application. By carefully computing the filter coefficients and adhering to standard conventions, it ensures reliable performance consistent with well-established digital signal processing practices.

---

This documentation should provide a comprehensive understanding of the `ButterN` filter, including detailed formulas and convention explanations, suitable for inclusion in your project's documentation.