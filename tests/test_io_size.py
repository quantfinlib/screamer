import numpy as np
from .param_cases import yield_test_cases, generate_array
from devtools import sii
import pytest

screamer_module = sii.load_screamer_module()

# Create a pytest parameterization using the collected test cases
@pytest.mark.parametrize(
    "class_name, params, array_type, array_length",
    yield_test_cases()
)
def test_vector(class_name, params, array_type, array_length):
    """Compare the output of screamer class and baseline reference implementation."""
    
    # Get screamer and baseline classes
    screamer_class = getattr(screamer_module, class_name, None)

    # Instantiate the screamer and baseline objects
    screamer_instance = screamer_class(**params)

    # Generate the input array for the test
    input_array = generate_array(array_type, array_length)

    # Run the batch version
    screamer_output = screamer_instance(input_array)

    # check that the input and output sizes are the same
    assert screamer_output.shape == input_array.shape



# Create a pytest parameterization using the collected test cases
@pytest.mark.parametrize(
    "class_name, params, array_type, array_length",
    yield_test_cases()
)
def test_matrix(class_name, params, array_type, array_length):
    """Compare the output of screamer class and baseline reference implementation."""
    
    # Get screamer and baseline classes
    screamer_class = getattr(screamer_module, class_name, None)

    # Instantiate the screamer and baseline objects
    screamer_instance = screamer_class(**params)

    # Generate the input matric for the test
    input_array =np.column_stack((
        generate_array(array_type, array_length),
        generate_array(array_type, array_length),
        generate_array(array_type, array_length)
    ))

    # Run the batch version
    screamer_output = screamer_instance(input_array)

    # check that the input and output sizes are the same
    assert screamer_output.shape == input_array.shape


# Create a pytest parameterization using the collected test cases
@pytest.mark.parametrize(
    "class_name, params, array_type, array_length",
    yield_test_cases()
)
def test_tensor(class_name, params, array_type, array_length):
    """Compare the output of screamer class and baseline reference implementation."""
    
    # Get screamer and baseline classes
    screamer_class = getattr(screamer_module, class_name, None)

    # Instantiate the screamer and baseline objects
    screamer_instance = screamer_class(**params)

    # Generate the input matric for the test
    input_array = np.column_stack((
        generate_array(array_type, array_length),
        generate_array(array_type, array_length),
        generate_array(array_type, array_length),
        generate_array(array_type, array_length),
        generate_array(array_type, array_length),
        generate_array(array_type, array_length)
    ))
    input_array = input_array.reshape(-1,2,3)

    # Run the batch version
    screamer_output = screamer_instance(input_array)

    # check that the input and output sizes are the same
    assert screamer_output.shape == input_array.shape


# Create a pytest parameterization using the collected test cases
@pytest.mark.parametrize(
    "class_name, params, array_type, array_length",
    yield_test_cases()
)
def test_view(class_name, params, array_type, array_length):
    """Compare the output of screamer class and baseline reference implementation."""
    
    # Get screamer and baseline classes
    screamer_class = getattr(screamer_module, class_name, None)

    # Instantiate the screamer and baseline objects
    screamer_instance = screamer_class(**params)

    # Generate the input matric for the test
    input_array = np.column_stack((
        generate_array(array_type, array_length),
        generate_array(array_type, array_length),
        generate_array(array_type, array_length),
        generate_array(array_type, array_length),
        generate_array(array_type, array_length),
        generate_array(array_type, array_length)
    ))
    input_array = input_array.reshape(-1,3)[::2,:]

    # Run the batch version
    screamer_output = screamer_instance(input_array)

    # check that the input and output sizes are the same
    assert screamer_output.shape == input_array.shape        