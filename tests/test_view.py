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
def test_screamer_view_strided(class_name, params, array_type, array_length):
    """Compare the output of screamer class and baseline reference implementation."""
    
    # Get screamer and baseline classes
    screamer_class = getattr(screamer_module, class_name, None)

    # Instantiate the screamer and baseline objects
    screamer_instance_1 = screamer_class(**params)
    screamer_instance_2 = screamer_class(**params)

    # Generate a 2N x 9 x 8 input matix
    arrays = []
    for _ in range(9*8):
        arrays.append(generate_array(array_type, 2*array_length))
    input_array = np.column_stack(arrays)
    input_array = input_array.reshape((-1, 9, 8))

    input_array_view = input_array[::2, ::3, ::4]

    # Run the streaming version
    screamer_output_1 = np.empty_like(input_array_view)
    for c1 in range(screamer_output_1.shape[1]):
        for c2 in range(screamer_output_1.shape[2]):
            screamer_instance_1.reset()
            screamer_output_1[:, c1, c2] = screamer_instance_1(input_array_view[:, c1, c2])

    # Run the matrix version
    screamer_output_2 = screamer_instance_2(input_array_view)

    np.testing.assert_allclose(
        screamer_output_1, screamer_output_2, rtol=1e-5, atol=1e-8,
        err_msg=f"Results do not match for {class_name}  with params {params} and array type '{array_type}' of length {array_length}"
    )


# Create a pytest parameterization using the collected test cases
@pytest.mark.parametrize(
    "class_name, params, array_type, array_length",
    yield_test_cases()
)
def test_screamer_view_strided_shifted(class_name, params, array_type, array_length):
    """Compare the output of screamer class and baseline reference implementation."""
    
    # Get screamer and baseline classes
    screamer_class = getattr(screamer_module, class_name, None)

    # Instantiate the screamer and baseline objects
    screamer_instance_1 = screamer_class(**params)
    screamer_instance_2 = screamer_class(**params)

    # Generate a 2N x 9 x 8 input matix
    arrays = []
    for _ in range(9*8):
        arrays.append(generate_array(array_type, 2*array_length))
    input_array = np.column_stack(arrays)
    input_array = input_array.reshape((-1, 9, 8))

    input_array_view = input_array[1::2, 2::3, 3::4]

    # Run the streaming version
    screamer_output_1 = np.empty_like(input_array_view)
    for c1 in range(screamer_output_1.shape[1]):
        for c2 in range(screamer_output_1.shape[2]):
            screamer_instance_1.reset()
            screamer_output_1[:, c1, c2] = screamer_instance_1(input_array_view[:, c1, c2])

    # Run the matrix version
    screamer_output_2 = screamer_instance_2(input_array_view)

    np.testing.assert_allclose(
        screamer_output_1, screamer_output_2, rtol=1e-5, atol=1e-8,
        err_msg=f"Results do not match for {class_name}  with params {params} and array type '{array_type}' of length {array_length}"
    )
