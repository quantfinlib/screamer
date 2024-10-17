# tests/test_python.py

import pytest
from screamer import Lag, lag_generator

def test_lag_valid_delay():
    lag = Lag(2, 0.0)
    assert lag(10.0) == 0.0
    assert lag(20.0) == 0.0
    assert lag(30.0) == 10.0
    assert lag(40.0) == 20.0

def test_lag_zero_delay():
    with pytest.raises(ValueError) as exc_info:
        Lag(0, 0.0)
    assert "Delay must be an integer >= 1." in str(exc_info.value)

def test_lag_negative_delay():
    with pytest.raises(ValueError) as exc_info:
        Lag(-1, 0.0)
    assert "Delay must be an integer >= 1." in str(exc_info.value)

def test_lag_generator_valid():
    values = [10, 20, 30, 40]
    delay = 2
    initial = 0.0
    gen = lag_generator(values, delay, initial)
    outputs = list(gen)
    assert outputs == [0.0, 0.0, 10.0, 20.0]

def test_lag_generator_invalid_delay():
    values = [10, 20]
    delay = 0
    initial = 0.0
    with pytest.raises(ValueError) as exc_info:
        list(lag_generator(values, delay, initial))
    assert "Delay must be an integer >= 1." in str(exc_info.value)
