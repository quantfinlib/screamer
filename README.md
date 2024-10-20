# Screamer

Screamingly fast financial technical indicators with C++ performance and Python simplicity, built for both time series analysis and real-time, event-driven streaming.

[![License](https://img.shields.io/pypi/l/screamer?color=#28A745)](https://github.com/quantfinlib/screamer/blob/main/LICENSE)
![Python Versions](https://img.shields.io/pypi/pyversions/screamer)
[![tests](https://github.com/quantfinlib/screamer/actions/workflows/ci.yml/badge.svg)](https://github.com/quantfinlib/screamer/actions/workflows/ci.yml)
[![Docs](https://readthedocs.org/projects/screamer/badge/?version=latest)](https://screamer.readthedocs.io/en/latest/?badge=latest) 
[![PyPI](https://img.shields.io/pypi/v/screamer)](https://pypi.org/project/screamer/)

Screamer is a high-performance Python library designed for efficient streaming indicator algorithms. Built with a core of optimized C++ code and integrated through Python bindings, Screamer delivers lightning-fast computations for real-time data processing. The library is perfect for real-time algorithmic trading applications that need low-latency indicators.

## Installation

```bash
pip install screamer
```

## Usage

### Streaming interface 

The streaming interface in screamer is designed for online stream processing, where data is processed element-by-element as it arrives, rather than working on the entire dataset at once. This is particularly useful when you’re dealing with real-time data, such as sensor readings, financial tick data, or other time-sensitive streams, where decisions or transformations need to be applied immediately as new data points become available.

In the example, we create a lag operator lag = screamer.lag(2, 0). This creates a lag stream with a delay of 2 and an initial value of 0. As new values are streamed into lag, it returns the value from 2 steps earlier.

```
import screamer

lag = screamer.lag(2, 0)

for x in [1,2,3,4]:
    print(lag(x))

---
0
0
1
2
```

### Transform interface

The transform interface in screamer is designed for processing a batch of data all at once, which can sometimes be more convenient and faster than processing one value at a time. Instead of working on each element individually in a loop, the entire array of values is passed to the transform function, and it applies the lag operation internally. This can lead to a speedup since the processing is done inside optimized C++ code rather than a Python loop.

In the example, we create the same lag operator as before, lag = screamer.lag(2, 0). This operator introduces a delay of 2 steps, just like in the streaming interface. However, instead of processing each value one by one, we pass a whole array of values (x = [1, 2, 3, 4]) to the lag.transform() function:

```
import screamer
import numpy as np

lag = screamer.lag(2, 0)

x = np.arange(1, 5)
y = lag.transform(x)

print('x =', x)
print('y =', y)
---
x = [1 2 3 4]
y = [0 0 1 2]
```
