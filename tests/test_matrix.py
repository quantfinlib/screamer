import numpy as np
from itertools import product
from .param_cases import yield_test_cases, generate_array
from devtools import load_screamer_module, baselines
import pytest

# Load the screamer module
screamer_module = load_screamer_module()

# Create a pytest parameterization using the collected test cases
@pytest.mark.parametrize(
    "class_name, params, array_type, array_length",
    yield_test_cases()
)
def test_screamer_matrix(class_name, params, array_type, array_length):
    """Compare the output of screamer class and baseline reference implementation."""
    
    # Get screamer and baseline classes
    screamer_class = getattr(screamer_module, class_name, None)

    # Instantiate the screamer and baseline objects
    screamer_instance_1 = screamer_class(**params)
    screamer_instance_2 = screamer_class(**params)

    # Generate a N x 3 input matix
    input_array = np.column_stack((
        generate_array(array_type, array_length),
        generate_array(array_type, array_length),
        generate_array(array_type, array_length)
    ))

    # Run the streaming version
    screamer_output_1 = np.empty_like(input_array)
    for c in range(3):
        screamer_instance_1.reset()
        screamer_output_1[:, c] = screamer_instance_1(input_array[:, c])

    # Run the matrix version
    screamer_output_2 = screamer_instance_2(input_array)

    np.testing.assert_allclose(
        screamer_output_1, screamer_output_2, rtol=1e-5, atol=1e-8,
        err_msg=f"Results do not match for {class_name}  with params {params} and array type '{array_type}' of length {array_length}"
    )
