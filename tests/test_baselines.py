import numpy as np
from itertools import product
from .param_cases import yield_test_cases_with_baselines, generate_array
from devtools import load_screamer_module, baselines
import pytest

# Load the screamer module
screamer_module = load_screamer_module()

# Create a pytest parameterization using the collected test cases
@pytest.mark.parametrize(
    "class_name, baseline_name, params, array_type, array_length",
    yield_test_cases_with_baselines()
)
def test_screamer_vs_baseline(class_name, baseline_name, params, array_type, array_length):
    """Compare the output of screamer class and baseline reference implementation."""
    
    # Get screamer and baseline classes
    screamer_class = getattr(screamer_module, class_name, None)
    baseline_class = getattr(baselines, baseline_name, None)

    if not screamer_class or not baseline_class:
        pytest.skip(f"Skipping {class_name} or {baseline_name} due to missing implementation")

    # Instantiate the screamer and baseline objects
    screamer_instance = screamer_class(**params)
    baseline_instance = baseline_class(**params)

    # Generate the input array for the test
    input_array = generate_array(array_type, array_length)

    # Run both implementations and compare outputs
    screamer_output = screamer_instance(input_array)
    baseline_output = baseline_instance(input_array)

    np.testing.assert_allclose(
        screamer_output[-10:], baseline_output[-10:], rtol=1e-5, atol=1e-8,
        err_msg=f"Results do not match for {class_name} vs {baseline_name} with params {params} and array type '{array_type}' of length {array_length}"
    )
