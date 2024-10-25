import numpy as np
import pytest

import importlib.util
import sys
import os

# Dynamically load the reference_impl s module
reference_impls_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "../reference_impls"))
spec = importlib.util.spec_from_file_location("reference_impls", os.path.join(reference_impls_path, "__init__.py"))
reference_impls = importlib.util.module_from_spec(spec)
sys.modules["reference_impls"] = reference_impls
spec.loader.exec_module(reference_impls)


test_args = {
    'array': np.random.normal(size=50),
    'window_size': 5,
    'quantile': 0.75
}

# Load the table with all reference functions
all_functions = reference_impls.all()
print('\n', all_functions)

def do_test_call(callable_name, args, **kwargs):
    """Dynamically calls the function from reference_impls and executes with provided arguments."""
    function_call = f'reference_impls.{callable_name}({args})'
    return eval(function_call, {'reference_impls': reference_impls}, kwargs)


@pytest.mark.parametrize("screamer_callable, reference_callable", [
    (screamer_fun_row['callable'], ref_fun_row['callable']) 
    for _, screamer_fun_row in all_functions[all_functions['lib'] == 'screamer'].iterrows()
    for _, ref_fun_row in all_functions[all_functions['func'] == screamer_fun_row['func']].iterrows()
    if ref_fun_row['lib'] != 'screamer'
])
def test_screamer_vs_reference(screamer_callable, reference_callable):
    """
    Parameterized test that compares the output of screamer functions against reference implementations.
    
    Parameters
    ----------
    screamer_callable : str
        The screamer library function name.
    reference_callable : str
        The reference implementation function name.
    """
    print('reference test:', screamer_callable, '<-->', reference_callable)

    # Run the screamer function
    screamer_result = do_test_call(screamer_callable, "array, window_size", **test_args)
    screamer_result_trimmed = screamer_result[test_args['window_size']:]
    
    # Run the reference function
    reference_result = do_test_call(reference_callable, "array, window_size", **test_args)
    reference_result_trimmed = reference_result[test_args['window_size']:]

    # Validate that both results are nearly the same (since we're working with numerical operations)
    np.testing.assert_allclose(
        screamer_result_trimmed, 
        reference_result_trimmed, 
        rtol=1e-5, atol=1e-8, 
        err_msg=f"Discrepancy between {screamer_callable} and {reference_callable}"
    )
