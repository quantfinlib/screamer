import screamer.screamer_bindings
from lib_info import collect_class_info
import importlib
import inspect
import numpy as np


# Dynamic test collection using the pytest_generate_tests hook
def pytest_generate_tests(metafunc):
    if 'ci' in metafunc.fixturenames:
        # Dynamically create the dictionary with metadata for the classes in our bindings
        class_meta = collect_class_info(screamer.screamer_bindings)

        # Parametrize the test based on the keys (class names) in the dictionary
        metafunc.parametrize("ci", class_meta.items())


# Test function that uses the dynamically parametrized class_name and class_info
def test_stream_vs_batch(ci):
    
    # unpack
    class_name, class_info = ci
    class_args = class_info.get('args',[])
    args = {arg['name']: arg['example'] for arg in class_args if 'example' in arg}

    module = importlib.import_module("screamer.screamer_bindings")
    cls = getattr(module, class_name) 

    # Instantiate the class with filtered arguments
    obj1 = cls(**args)
    
    # generate some input
    input = np.cos(np.arange(100))


    output1 = np.zeros_like(input)
    for i in range(100):
        output1[i] = obj1(input[i])

    # Vectorized transform
    obj2 = cls(**args)
    if hasattr(cls, 'transform'):
        output2 = obj2.transform(input)
    else:
        output2 = obj2(input)

    # Should be the same
    print('args', args)
    print('output1', output1)
    print('output2', output2)

    np.testing.assert_allclose(
        output1, 
        output2, 
        rtol=1e-5, atol=1e-8, 
        err_msg=f"Discrepancy between streaming and batch mode for {class_name}"
    )


