import screamer.screamer_bindings
from lib_info import collect_class_info
import pytest
import importlib

# Dynamic test collection using the pytest_generate_tests hook
def pytest_generate_tests(metafunc):
    if 'class_name' in metafunc.fixturenames and 'class_info' in metafunc.fixturenames:
        # Dynamically create the dictionary with metadata
        class_meta = collect_class_info(screamer.screamer_bindings)

        # Parametrize the test based on the keys (class names) in the dictionary
        metafunc.parametrize("class_name,class_info", class_meta.items())

# Test function that uses the dynamically parametrized class_name and class_info
def test_negative_window_size(class_name, class_info):
    # Check if the class has a 'window_size' argument
    has_window_size = any(arg['name'] == 'window_size' for arg in class_info['args'])

    if has_window_size:
        # Dynamically import the module
        module = importlib.import_module("screamer.screamer_bindings")
        cls = getattr(module, class_name)

        # Test the invalid window_size (-1) raises ValueError
        with pytest.raises(ValueError):
            obj = cls(-1)

# Test function that uses the dynamically parametrized class_name and class_info
def test_zero_window_size(class_name, class_info):
    # Check if the class has a 'window_size' argument
    has_window_size = any(arg['name'] == 'window_size' for arg in class_info['args'])

    if has_window_size:
        # Dynamically import the module
        module = importlib.import_module("screamer.screamer_bindings")
        cls = getattr(module, class_name)

        # Test the invalid window_size (0) raises ValueError
        with pytest.raises(ValueError):
            obj = cls(0)            

# Test function that uses the dynamically parametrized class_name and class_info
def test_politive_window_size(class_name, class_info):
    # Check if the class has a 'window_size' argument
    has_window_size = any(arg['name'] == 'window_size' for arg in class_info['args'])

    if has_window_size:
        # Dynamically import the module
        module = importlib.import_module("screamer.screamer_bindings")
        cls = getattr(module, class_name)

        # Test the valid window_size
        obj = cls(1)    