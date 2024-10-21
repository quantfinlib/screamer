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


# Test function that uses the dynamically parametrized class_name and class_info
def test_matrix(class_name):
    module = importlib.import_module("screamer.screamer_bindings")
    cls = getattr(module, class_name) 
      
    # generate some 4 column matrix input
    input = np.cos(np.arange(240)).reshape(-1, 4)

    # Loop over columns
    obj1 = cls(10)
    output1 = np.zeros_like(input)
    for col in range(input.shape[1]):
        output1[:, col] = obj1.transform(input[:, col])

    # Matrix in one go
    obj2 = cls(10)
    output2 = obj2.transform(input)

    assert output1.shape == input.shape
    assert output1.shape == output2.shape

    # Should be exactly the same
    output1 = output1.flatten()
    output2 = output2.flatten()
    assert np.all([np.isnan(x1) if np.isnan(x2) else x1 == x2 for x1, x2 in zip(output1, output2)])


# Test function that uses the dynamically parametrized class_name and class_info
def test_tensor(class_name):
    module = importlib.import_module("screamer.screamer_bindings")
    cls = getattr(module, class_name) 
      
    # generate some 3d tensor input
    input = np.cos(np.arange(240)).reshape(-1, 2, 3)

    # Loop over columns
    obj1 = cls(10)
    output1 = np.zeros_like(input)
    for d1 in range(input.shape[1]):
        for d2 in range(input.shape[2]):
            output1[:, d1, d2] = obj1.transform(input[:, d1, d2])

    # Process tensor in one go
    obj2 = cls(10)
    output2 = obj2.transform(input)

    assert output1.shape == input.shape
    assert output1.shape == output2.shape

    # Should be exactly the same
    output1 = output1.flatten()
    output2 = output2.flatten()
    assert np.all([np.isnan(x1) if np.isnan(x2) else x1 == x2 for x1, x2 in zip(output1, output2)])



