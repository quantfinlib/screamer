import pytest
from screamer import RollingSum
import numpy as np

def test_rolling_sum():
    trans = RollingSum(2)
    assert trans(10.0) == 10.0
    assert trans(5.0) == 15.0
    assert trans(np.nan) == 5.0
    assert trans(10.0) == 10.0


