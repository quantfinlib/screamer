import logging
logging.basicConfig(encoding='utf-8', level=logging.DEBUG)
logger = logging.getLogger(__name__)
logger.info(f'entering devtools/__init__.py')

import sys

logger.info(f'sys.path:')
for p in sys.path:
    logger.info(f'- {p}')

import glob
import importlib.util
import inspect
import re
import os
import sys
import importlib
import os
import devtools.baselines

local_project_path = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
logger.info(f'local_project_path: {local_project_path}')

local_screamer_path = os.path.join(local_project_path, 'screamer')
logger.info(f'local_screamer_path: {local_screamer_path}')

def remove_local_screamer_path():
    while local_screamer_path in sys.path:
        sys.path.remove(local_screamer_path)

def add_local_screamer_path():
    while local_screamer_path in sys.path:
        sys.path.remove(local_screamer_path)
    sys.path.insert(0, local_screamer_path)

def load_local_screamer_bindings():
    add_local_screamer_path()
    # Locate the compiled module (.so, .pyd, or platform-specific extensions)
    possible_extensions = ['*.so', '*.pyd', '*.dll']
    for ext in possible_extensions:
        so_files = glob.glob(os.path.join(local_screamer_path, f'screamer_bindings{ext}'))
        if so_files:
            so_file_path = so_files[0]
            logger.info(f'found local bindings: {so_file_path}')
            break
    else:
        logger.info(f'No local bindings found.')
        return None

    # Load the module
    module_name = 'screamer_bindings'
    spec = importlib.util.spec_from_file_location(module_name, so_file_path)
    logger.info(f'loading local module from spec: {spec}')
    module = importlib.util.module_from_spec(spec)
    sys.modules['screamer_bindings'] = module  # Ensure it's available globally
    sys.modules['screamer'] = module  # Ensure it's available globally
    spec.loader.exec_module(module)
    logger.info(f'done loading {spec}')
    return module

def load_env_screamer_bindings():
    spec = importlib.util.find_spec("screamer")
    if spec is None:
        logger.info(f'Unable to find screamer module in env')
        return None
    module = importlib.util.module_from_spec(spec)
    sys.modules['screamer_bindings'] = module  # Ensure it's available globally
    sys.modules['screamer'] = module  # Ensure it's available globally
    spec.loader.exec_module(module)
    logger.info(f'done loading {spec}')
    return module
    

    try:
        remove_local_screamer_path()
        import screamer
        add_local_screamer_path()
        logger.info(f'Done loading screamer from env {screamer.__version__}')
        print('load_env_screamer_bindings')
        return screamer
    
    except ImportError as e:
        logger.info(f'Failed to load the screamer module from env.')
        raise ImportError(
            "Failed to load the screamer module. Ensure that the library is "
            "installed as a wheel or that the compiled binaries are built locally."
        ) from e

def load_screamer_module():
    try:
        # First, try to load locally compiled binary
        return load_local_screamer_bindings()
    except FileNotFoundError:
        # Fallback: attempt to load from installed site-packages
        try:
            load_env_screamer_bindings()
        except ImportError as e:
            raise ImportError(
                "Failed to load the screamer module. Ensure that the library is "
                "installed as a wheel or that the compiled binaries are built locally."
            ) from e


def get_module_classes(module):
    class_names = [name for name, obj in inspect.getmembers(module, inspect.isclass)]
    return class_names

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