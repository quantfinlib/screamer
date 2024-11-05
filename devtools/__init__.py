import devtools.baselines
import glob
import importlib.util
import inspect
import re
import os
import sys
import importlib
import os

# Print the file path for debugging
print(f"Loading devtools from: {os.path.abspath(__file__)}")


def load_screamer_compiled_screamer_bindings():
    # Locate the compiled module (.so, .pyd, or platform-specific extensions)
    possible_extensions = ['*.so', '*.pyd', '*.dll']
    for ext in possible_extensions:
        so_files = glob.glob(os.path.join('screamer', f'screamer_bindings{ext}'))
        if so_files:
            so_file_path = so_files[0]
            break
    else:
        raise FileNotFoundError("Compiled module 'screamer_bindings' not found.")

    # Load the module
    module_name = 'screamer_bindings'
    spec = importlib.util.spec_from_file_location(module_name, so_file_path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[module_name] = module  # Ensure it's available globally
    spec.loader.exec_module(module)
    return module


def load_screamer_module():
    try:
        # First, try to load locally compiled binary
        return load_screamer_compiled_screamer_bindings()
    except FileNotFoundError:
        # Fallback: attempt to load from installed site-packages
        try:
            import screamer
            return screamer
        except ImportError as e:
            raise ImportError(
                "Failed to load the screamer module. Ensure that the library is "
                "installed as a wheel or that the compiled binaries are built locally."
            ) from e


def get_module_classes(module):
    return [name for name, obj in inspect.getmembers(module, inspect.isclass)]

def get_baselines(base_name='Linear'):
    implementations = []
    for name, obj in inspect.getmembers(devtools.baselines, inspect.isclass):
        if name.startswith(base_name + '_'):
            implementations.append(name)
    return implementations

def get_constructor_arguments(cls):
    """Extract constructor arguments and their types from a class's __init__ method."""
    init_method = cls.__init__
    if not init_method or not init_method.__doc__:
        return None

    # Extract the argument list from the docstring using regex
    signature_match = re.search(r'__init__\(self.*?\)\s*->.*', init_method.__doc__)
    if not signature_match:
        return None

    signature_line = signature_match.group(0)

    # Extract argument names and types from the signature line
    arg_pattern = re.compile(r'(\w+): (\w+)')
    args = arg_pattern.findall(signature_line)

    # Extract argument descriptions from :param sections
    param_pattern = re.compile(r':param (\w+): (.*)')
    param_descriptions = {name: desc for name, desc in param_pattern.findall(init_method.__doc__)}

    # Combine the arguments, types, and descriptions
    arg_info = []
    for name, arg_type in args:
        if name != 'self':  # Ignore 'self'
            arg_info.append((name, arg_type))

    return arg_info