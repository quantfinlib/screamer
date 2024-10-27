import screamer.screamer_bindings
from lib_info import collect_class_info
import pytest
import importlib
import numpy as np


# Dynamic test collection using the pytest_generate_tests hook
def pytest_generate_tests(metafunc):
    if 'ci' in metafunc.fixturenames:
        # Dynamically create the dictionary with metadata
        class_meta = collect_class_info(screamer.screamer_bindings)

        # Parametrize the test based on the keys (class names) in the dictionary
        metafunc.parametrize("ci", class_meta.items())


# Test processing a matrix columns wise, vs in one go with .transform()
def test_matrix(ci):
    class_name, class_info = ci
    args = {arg['name']: arg['example'] for arg in class_info.get('args',[])}

    # Instantiate an indicator class
    module = importlib.import_module("screamer.screamer_bindings")
    cls = getattr(module, class_name) 
      
    # generate some 4 column matrix input
    input = 1.1*np.arange(24).reshape(-1, 4)

    # Loop over columns
    obj1 = cls(**args)
    output1 = np.zeros_like(input, dtype=float)
    for col in range(input.shape[1]):
        if hasattr(obj1, 'transform'):
            output1[:, col] = obj1.transform(input[:, col].copy())
        else:
            output1[:, col] = obj1(input[:, col].copy())

    # Matrix in one go
    obj2 = cls(**args)
    if hasattr(obj2, 'transform'):
        output2 = obj2.transform(input)
    else:
        output2 = obj2(input)

    # Should be exactly the same
    np.testing.assert_array_equal(output1, output2, err_msg="matrices do not match")


# Test processing a tensor columns wise, vs in one go with .transform()
def test_tensor(ci):
    class_name, class_info = ci
    args = {arg['name']: arg['example'] for arg in class_info.get('args',[])}


    # Instantiate an indicator class
    module = importlib.import_module("screamer.screamer_bindings")
    cls = getattr(module, class_name) 
      
    # generate some 3d tensor input
    input = np.arange(24).reshape(-1, 2, 3)

    # Loop over columns
    obj1 = cls(**args)
    output1 = np.zeros_like(input, dtype=float)
    for d1 in range(input.shape[1]):
        for d2 in range(input.shape[2]):
            if hasattr(obj1, 'transform'):
                output1[:, d1, d2] = obj1.transform(input[:, d1, d2])
            else:
                output1[:, d1, d2] = obj1(input[:, d1, d2])

    # Process tensor in one go
    obj2 = cls(**args)
    if hasattr(obj2, 'transform'):
        output2 = obj2.transform(input)
    else:
        output2 = obj2(input)

    # Should be exactly the same
    np.testing.assert_array_equal(output1, output2, err_msg="tensors do not match")

