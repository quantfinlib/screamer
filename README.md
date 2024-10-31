# Screamer


```bash
pip install screamer
```


Screamer is a high-performance Python library for time series analysis, designed for speed, 
accuracy, and versatility in handling both NumPy arrays and streaming data. 


[![License](https://img.shields.io/pypi/l/screamer?color=#28A745)](https://github.com/quantfinlib/screamer/blob/main/LICENSE)
![Python Versions](https://img.shields.io/pypi/pyversions/screamer)
[![tests](https://github.com/quantfinlib/screamer/actions/workflows/test.yml/badge.svg)](https://github.com/quantfinlib/screamer/actions/workflows/test.yml)
[![Docs](https://readthedocs.org/projects/screamer/badge/?version=latest)](https://screamer.readthedocs.io/en/latest/?badge=latest) 
[![PyPI](https://img.shields.io/pypi/v/screamer)](https://pypi.org/project/screamer/)


Engineered in C++ and leveraging state-of-the-art numerical algorithms, Screamer delivers 
exceptional computational efficiency, consistently outperforming traditional libraries 
like NumPy and Pandas—often by factors of two or more, and in some cases by orders of magnitude. 

Screamer's polymorphic design allows all functions to operate seamlessly on both static arrays
and streaming data, enabling smooth integration and consistent syntax without code modification. 
Built for secure, real-time analysis, Screamer's stream-processing approach ensures every 
function is free from look-ahead bias, guaranteeing accurate results you can trust.

For detailed documentation, visit [Screamer Documentation on ReadTheDocs](https://screamer.readthedocs.io/en/latest/).
