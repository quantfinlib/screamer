Screamer
========

.. code-block:: console

  pip install screamer


Screamer is a high-performance Python library for time series analysis, designed for speed, 
accuracy, and versatility in handling both NumPy arrays and streaming data. 


.. image:: https://img.shields.io/pypi/l/screamer?color=#28A745
   :target: https://github.com/quantfinlib/screamer/blob/main/LICENSE
   :alt: License

.. image:: https://img.shields.io/pypi/pyversions/screamer
   :alt: Python Versions

.. image:: https://github.com/quantfinlib/screamer/actions/workflows/test.yml/badge.svg
   :target: https://github.com/quantfinlib/screamer/actions/workflows/test.yml
   :alt: Tests

.. image:: https://readthedocs.org/projects/screamer/badge/?version=latest
   :target: https://screamer.readthedocs.io/en/latest/?badge=latest
   :alt: Documentation

.. image:: https://img.shields.io/pypi/v/screamer
   :target: https://pypi.org/project/screamer/
   :alt: PyPI






Here is an example that does a least squared fit a 2nd degree polynomial `y = ax^2 + bx + c` to a
sliding window of length 50, and then return the slope of that polynomial. The slope gets fed into a 
second stream processor that returns the sign of that slope (upward or downward trending).

.. image:: /img/3lines.png
   :target: /img/3lines.png
   :alt: Three lines of code by Herminio Vazquez
   :width: 170px
   :align: right

.. code-block:: python
   :emphasize-lines: 6

    from screamer import RollingPoly2, Sign

    slope = RollingPoly2(window_size=50, derivative_order=1)
    sign = Sign()

    result = sign(slope(data))


The plot below shows the input data (top, blue), the slope fitted to the previous 50 samples (middle, orange),
and the sign of the slope (bottom, red).


.. plotly::
   :include-source: False

    import numpy as np
    import plotly.graph_objects as go
    from plotly.subplots import make_subplots
    from screamer import RollingPoly2, Sign

    # Generate example data
    np.random.seed(0)
    data = np.cumsum(np.random.normal(size=300))

    # Create subplots with specified row heights and shared x-axis
    fig = make_subplots(
        rows=3, cols=1,
        shared_xaxes=True,
        row_heights=[1/3, 1/3, 1/3],
        vertical_spacing=0.02
    )

    # rolling mean
    slope = RollingPoly2(window_size=50, derivative_order=1)

    # sign
    sign = Sign()

    # Add traces for input data and results with different derivative orders
    fig.add_trace(go.Scatter(y=data, mode='lines', name='data'), row=1, col=1)
    fig.add_trace(go.Scatter(y=slope(data), mode='lines', name='slope(data)', line=dict(color='orange')), row=2, col=1)
    fig.add_trace(go.Scatter(y=sign(slope(data)), mode='lines', name='sign(slope(data))', line=dict(color='red')), row=3, col=1)

    # Update layout with titles and axis labels
    fig.update_layout(
        title=None,
        xaxis3_title="Sample index",
        yaxis=dict(title="Data"),
        yaxis2=dict(title="Moving Average"),
        yaxis3=dict(title="Sign"),
         margin=dict(l=20, r=20, t=20, b=20)  # Adjust left, right, top, and bottom margins        
    )

    fig.show()



Engineered in C++ and leveraging state-of-the-art numerical algorithms, Screamer delivers 
exceptional computational efficiency, consistently outperforming traditional libraries 
like NumPy and Pandas—often by factors of two or more, and in some cases by orders of magnitude. 

.. image:: /img/speed.png
   :target: /img/speed.png
   :alt: speed comparison


Screamer's polymorphic design allows all functions to operate seamlessly on both static arrays
and streaming data, enabling smooth integration and consistent syntax without code modification. 
Built for secure, real-time analysis, Screamer's stream-processing approach ensures every 
function is free from look-ahead bias, guaranteeing accurate results you can trust.





.. include:: usage.md
   :parser: myst_parser.sphinx_

.. toctree::
   :maxdepth: 1
   :caption: Main
   :hidden:
   
   usage
   changelog


.. toctree::
   :maxdepth: 1
   :caption: Math
   :hidden:

   math/Abs
   math/Erf
   math/Erfc
   math/Exp
   math/Log
   math/Sign
   math/Sqrt


.. toctree::
   :maxdepth: 1
   :caption: Preprocessing
   :hidden:

   preprocessing/Clip
   preprocessing/Ffill
   preprocessing/FillNa

.. toctree::
   :maxdepth: 1
   :caption: Rolling Functions
   :hidden:

   rolling/RollingMax
   rolling/RollingMean
   rolling/RollingMedian
   rolling/RollingMin
   rolling/RollingKurt
   rolling/RollingPoly1
   rolling/RollingPoly2
   rolling/RollingRms
   rolling/RollingSkew
   rolling/RollingStd
   rolling/RollingSum
   rolling/RollingVar
   rolling/RollingQuantile
   rolling/RollingZscore

