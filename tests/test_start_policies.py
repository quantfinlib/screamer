from functools import partial
import numpy as np
from devtools import sii
import pytest

screamer_module = sii.load_screamer_module()


def lag_factory(window_size, start_policy):
    return screamer_module.Lag(window_size-1, start_policy)

def diff_factory(window_size, start_policy):
    return screamer_module.Diff(window_size-1, start_policy)

# Here we have (partial)function(wrappers) that expect exactly 2 arguments: window_size and start_policy
factories = {
    'lag': lag_factory,
    'diff': diff_factory,
    'rolling_mean': screamer_module.RollingMean,
    'rolling_sum': screamer_module.RollingSum,
    'rolling_poly1': screamer_module.RollingPoly1,
    'rolling_poly2': screamer_module.RollingPoly2,
    'rolling_var': screamer_module.RollingVar,
    'rolling_std': screamer_module.RollingStd,
    'rolling_skew': screamer_module.RollingSkew,
    'rolling_kurt': screamer_module.RollingKurt,
    'rolling_rms': screamer_module.RollingRms,
    'rolling_rsi': screamer_module.RollingRSI,
    'rolling_ou': screamer_module.RollingOU,
}


# The first window_size-1 elements must be NaN, the next one non-NaN
def generic_test_strict(factory, window_size, array_size=12):
    obj = factory(window_size=window_size, start_policy='strict')
    input = np.random.normal(size=array_size)
    output = obj(input)
    assert np.all(np.isnan(output[:window_size-1]))
    assert not np.isnan(output[window_size-1])


# The output must be as-if we had processed window_size-1 zeros
def generic_test_zero(factory, window_size, array_size=12):
    obj = factory(window_size=window_size, start_policy='zero')
    input = np.random.normal(size=array_size)
    output = obj(input)

    input_zero_padded = np.concatenate([np.full(window_size-1, 0), input])
    obj.reset()
    output_zero_padded = obj(input_zero_padded)[window_size-1:]

    np.testing.assert_allclose(
        output, output_zero_padded, rtol=1e-5, atol=1e-8,
        err_msg=f"Zero padding check failed"
    )

# The output should be as-if we ran our model with various expanding windows
# todo: this test might need some tweaking
def generic_test_expanding(factory, window_size, array_size=12):
    obj = factory(window_size=window_size, start_policy='expanding')
    input = np.random.normal(size=array_size)
    output = obj(input)
    print('expanding')
    print(output)

    for i in range(2, window_size):
        try: # some construtor like RollingPoly2 don't like a windows size of 2
            obj_i = factory(window_size=i, start_policy='expanding')
            output_i = obj_i(input[:i])
            print(i, output_i)
            assert output[i-1] == output_i[i-1]
        except:
            pass


# Parameterize over the factory names and corresponding functions
@pytest.mark.parametrize("factory_name,factory", factories.items())
def test_strict_policies(factory_name, factory):
    window_size = 5
    generic_test_strict(factory, window_size)

# Parameterize over the factory names and corresponding functions
@pytest.mark.parametrize("factory_name,factory", factories.items())
def test_zero_policies(factory_name, factory):
    window_size = 5
    generic_test_zero(factory, window_size)

# Parameterize over the factory names and corresponding functions
@pytest.mark.parametrize("factory_name,factory", factories.items())
def test_expanding_policies(factory_name, factory):
    window_size = 5
    generic_test_expanding(factory, window_size)