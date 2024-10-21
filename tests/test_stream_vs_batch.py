import screamer.screamer_bindings
from lib_info import collect_class_info
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
def test_stream_vs_batch(class_name):
    module = importlib.import_module("screamer.screamer_bindings")
    cls = getattr(module, class_name) 
      
    # generate some input
    input = np.cos(np.arange(100))

    # Stream loop
    obj1 = cls(10)
    output1 = np.zeros_like(input)
    for i in range(100):
        output1[i] = obj1(input[i])

    # Vectorized transform
    obj2 = cls(10)
    output2 = obj2.transform(input)

    # Should be exactly the same
    assert np.all([np.isnan(x1) if np.isnan(x2) else x1 == x2 for x1, x2 in zip(output1, output2)])




