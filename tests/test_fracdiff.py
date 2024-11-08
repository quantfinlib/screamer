from screamer import RollingFracDiff
import numpy as np
import pytest


@pytest.fixture
def prices():
    np.random.seed(42)
    return np.random.lognormal(0, 0.1, 10000)


def test_frac_diff_generic(prices):
    fracdiff = RollingFracDiff(frac_order=0, window_size=10, threshold=1e-6)
    fs = np.array([fracdiff(p) for p in prices])
    fs2 = fracdiff(prices)
    assert np.allclose(fs[np.isfinite(fs2)], fs2[np.isfinite(fs2)])


def test_frac_diff_identity(prices):
    fracdiff = RollingFracDiff(frac_order=0, window_size=10, threshold=1e-6)
    fs = np.array([fracdiff(p) for p in prices])
    assert np.allclose(fs, prices)


def test_frac_diff_d_equal_one(prices):
    fracdiff = RollingFracDiff(frac_order=1.0, window_size=10, threshold=1e-6)
    fs = fracdiff(prices)
    assert np.allclose(
        fs[1:], np.diff(prices)
    ), "when d=1, the output should be the same as the diff of the input."
    assert fs[0] == prices[0], "First value should be the same as the input."


def test_frac_diff_d_equal_two(prices):
    fracdiff = RollingFracDiff(frac_order=2.0, window_size=10, threshold=1e-6)
    fs = fracdiff(prices)
    expected = prices[2:] - 2 * prices[1:-1] + prices[:-2]
    assert np.allclose(
        fs[2:], expected
    ), "when d=2, the output should be the same as p(t+2) - 2 p(t-1) + p(t)."
    expected_first = prices[1] - 2 * prices[0]
    assert fs[1] == expected_first, "Second value should be the same as p1 - 2p0."
    assert fs[0] == prices[0], "First value should be the same as the input."
