import screamer.screamer_bindings
from lib_info import collect_class_info
import pytest
import importlib
import numpy as np


# Dynamic test collection using the pytest_generate_tests hook
def pytest_generate_tests(metafunc):
    if 'class_name' in metafunc.fixturenames:
        # Dynamically create the dictionary with metadata
        class_meta = collect_class_info(screamer.screamer_bindings)

        # Parametrize the test based on the keys (class names) in the dictionary
        metafunc.parametrize("class_name", class_meta.keys())


# Test processing a matrix columns wise, vs in one go with .transform()
def test_matrix(class_name):
    if class_name == "FracDiff":
        arg = (0.1, 10, 1e-6)
    else:
        # windows_size or delay
        arg = (3,)

    # Instantiate an indicator class
    module = importlib.import_module("screamer.screamer_bindings")
    cls = getattr(module, class_name) 
    
    # generate some 4 column matrix input
    input = 1.1*np.arange(24).reshape(-1, 4)

    # Loop over columns
    obj1 = cls(*arg)
    output1 = np.zeros_like(input, dtype=float)
    for col in range(input.shape[1]):
        output1[:, col] = obj1.transform(input[:, col].copy())

    # Matrix in one go
    obj2 = cls(*arg)
    output2 = obj2.transform(input)

    # Should be exactly the same
    np.testing.assert_array_equal(output1, output2, err_msg="matrices do not match")


# Test processing a tensor columns wise, vs in one go with .transform()
def test_tensor(class_name):

    if class_name == "FracDiff":
        arg = (0.1, 10, 1e-6)
    else:
        # windows_size or delay
        arg = (3,)

    # Instantiate an indicator class
    module = importlib.import_module("screamer.screamer_bindings")
    cls = getattr(module, class_name) 
      
    # generate some 3d tensor input
    input = np.arange(24).reshape(-1, 2, 3)

    # Loop over columns
    obj1 = cls(*arg)
    output1 = np.zeros_like(input, dtype=float)
    for d1 in range(input.shape[1]):
        for d2 in range(input.shape[2]):
            output1[:, d1, d2] = obj1.transform(input[:, d1, d2])

    # Process tensor in one go
    obj2 = cls(*arg)
    output2 = obj2.transform(input)

    # Should be exactly the same
    np.testing.assert_array_equal(output1, output2, err_msg="tensors do not match")

