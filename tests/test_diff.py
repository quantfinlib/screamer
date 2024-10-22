import pytest
from screamer import Diff, diff_generator
import numpy as np

def test_diff_valid_delay():
    diff = Diff(2)
    assert np.isnan(diff(10.0))  # Use np.isnan() to check for NaN
    assert np.isnan(diff(20.0))  # Use np.isnan() to check for NaN
    assert diff(30.0) == 20.0
    assert diff(40.0) == 20.0


def test_diff_zero_delay():
    with pytest.raises(ValueError) as exc_info:
        Diff(0)


def test_diff_negative_delay():
    with pytest.raises(ValueError) as exc_info:
        Diff(-1)


def test_diff_generator_valid():
    values = [10, 20, 30, 40]
    delay = 2
    gen = diff_generator(values, delay)
    outputs = list(gen)
    
    # Compare lists with NaN values correctly
    expected_output = [np.nan, np.nan, 20.0, 20.0]
    assert np.all([np.isnan(o) if np.isnan(e) else o == e for o, e in zip(outputs, expected_output)])


def test_diff_generator_invalid_delay():
    values = [10, 20]
    delay = 0
    with pytest.raises(ValueError) as exc_info:
        list(diff_generator(values, delay))


def test_diff_reset():
    diff = Diff(2)
    assert np.isnan(diff(10.0))  # Use np.isnan() to check for NaN
    assert np.isnan(diff(20.0))  # Use np.isnan() to check for NaN
    assert diff(30.0) == 20.0
    diff.reset()
    assert np.isnan(diff(40.0))  # Use np.isnan() to check for NaN
    assert np.isnan(diff(50.0))  # Use np.isnan() to check for NaN
    assert diff(60.0) == 20.0


def test_diff_transform():
    values = np.array([10, 20, 30, 40])
    diff = Diff(2)
    outputs = diff.transform(values)
    
    # Use np.testing.assert_array_equal with NaN handling
    expected_output = np.array([np.nan, np.nan, 20.0, 20.0])
    np.testing.assert_array_equal(outputs, expected_output, err_msg="Arrays do not match")


def test_diff_transform_2d():
    values = np.array([
        [10, 100, 1000], 
        [20, 200, 2000], 
        [30, 300, 3001], 
        [40, 400, 4003]])
    expected_output = np.array([
        [np.nan, np.nan, np.nan], 
        [np.nan, np.nan, np.nan], 
        [20, 200, 2001], 
        [20, 200, 2003]])

    diff = Diff(2)
    outputs = diff.transform(values)
    
    # Compare 2D arrays with NaN
    np.testing.assert_array_equal(outputs, expected_output, err_msg="2D arrays do not match")


def test_diff_transform_3d():
    values = np.random.normal(size=(4, 3, 2))
    expected_output = np.zeros_like(values)
    expected_output[2:, ...] = values[2:, ...] - values[:-2, ...]
    expected_output[:2, ...] = np.nan

    diff = Diff(2)
    outputs = diff.transform(values)
    
    # Compare 3D arrays with NaN
    np.testing.assert_array_equal(outputs, expected_output, err_msg="3D arrays do not match")
