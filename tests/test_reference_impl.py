import numpy as np
import pytest
import importlib.util
import sys
import os

# Dynamically load the reference_impls module
# This block constructs an absolute path to the reference implementations directory, loads it as a Python module,
# and makes it accessible under the module name "reference_impls" for subsequent imports and function calls.

reference_impls_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "../reference_impls"))
spec = importlib.util.spec_from_file_location("reference_impls", os.path.join(reference_impls_path, "__init__.py"))
reference_impls = importlib.util.module_from_spec(spec)
sys.modules["reference_impls"] = reference_impls
spec.loader.exec_module(reference_impls)

# Define a dictionary with test arguments and their default values.
# These are used for running functions in both screamer and reference implementations.
test_args = {
    'array': np.random.normal(size=40),   # Random array of size 40 as the main data input
    'window_size': 10,                    # Window size for operations
    'delay': 10,                          # Delay parameter, if required
    'fill': 0.0,                          # Fill value for any gaps
    'weight': 0.8,                        # Weight parameter, if applicable
    'quantile': 0.75,                     # Quantile parameter, if applicable
    'lower': -0.1,
    'upper': 0.5
}

# Load a DataFrame from the reference_impls module containing metadata on all reference functions
# This table lists function names, arguments, and other details necessary for dynamic testing.
all_functions = reference_impls.all()
print('\n', all_functions)

def do_test_call(callable_name, args, **kwargs):
    """Dynamically calls a function from the reference_impls module with the provided arguments.

    Parameters:
    - callable_name (str): The name of the function to call in the reference_impls module.
    - args (str): A string of comma-separated argument names for the function.
    - kwargs: The actual values for the args.

    Returns:
    - The result of executing the dynamically called function.
    """
    function_call = f'reference_impls.{callable_name}({args})'
    return eval(function_call, {'reference_impls': reference_impls}, kwargs)


@pytest.mark.parametrize(
    "screamer_callable, screamer_arg_names, reference_callable, reference_arg_names", 
    [
        # Create parameterized test cases for each screamer function vs. corresponding reference function
        (
            screamer_fun_row['callable'], 
            screamer_fun_row['args'], 
            ref_fun_row['callable'], 
            ref_fun_row['args']
        ) 
        # Iterate over each function in screamer, pairing it with each non-screamer function of the same type.
        for _, screamer_fun_row in all_functions[all_functions['lib'] == 'screamer'].iterrows()
        for _, ref_fun_row in all_functions[all_functions['func'] == screamer_fun_row['func']].iterrows()
        if ref_fun_row['lib'] != 'screamer'  # Ensure we only compare screamer functions to other implementations
    ]
)
def test_screamer_vs_reference(screamer_callable, screamer_arg_names, reference_callable, reference_arg_names):
    """Test a screamer function against a reference implementation.

    Parameters:
    - screamer_callable (str): The name of the screamer function to call.
    - screamer_arg_names (str): A comma-separated string of argument names for the screamer function.
    - reference_callable (str): The name of the reference function to call.
    - reference_arg_names (str): A comma-separated string of argument names for the reference function.
    """
    
    # Prepare and call the screamer function
    screamer_args = {arg: test_args[arg] for arg in screamer_arg_names.split(',')}
    screamer_result = do_test_call(screamer_callable, screamer_arg_names, **screamer_args)
    screamer_result_trimmed = screamer_result[test_args['window_size']:]  # Trim the initial window for consistency

    # Prepare and call the reference function
    reference_args = {arg: test_args[arg] for arg in reference_arg_names.split(',')}
    reference_result = do_test_call(reference_callable, reference_arg_names, **reference_args)
    reference_result_trimmed = reference_result[test_args['window_size']:]

    # Validate that both results are nearly the same within a relative and absolute tolerance.
    # The assert_allclose test is used for numerical stability verification.
    np.testing.assert_allclose(
        screamer_result_trimmed, 
        reference_result_trimmed, 
        rtol=1e-5, atol=1e-8, 
        err_msg=f"Discrepancy between {screamer_callable} and {reference_callable}"
    )
