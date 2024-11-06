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
import glob

local_project_path = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
logger.info(f'local_project_path: {local_project_path}')

local_screamer_path = os.path.join(local_project_path, 'screamer')
logger.info(f'local_screamer_path: {local_screamer_path}')

def find_first_site_packages_path():
    for p in sys.path:
        if '/venv/lib/' in p:

            logger.info(f'found a venv site_packages: {p}')
            logger.info('packages in venv:')
            for f in glob.glob(os.path.join(p, '*')):
                logger.info(f'- {f}')
            logger.info('files in screamer package in venv:')
            for f in glob.glob(os.path.join(p, 'screamer/*')):
                logger.info(f'- {f}')        
            logger.info('done.')

            return p
    return None

venv_site_packages_path = find_first_site_packages_path()

def log_sys_path():
    logger.info(f'sys.path:')
    for p in sys.path:
        logger.info(f'- {p}')

def log_pythonpath():
    try:
        python_paths = os.environ['PYTHONPATH'].split(os.pathsep)
    except KeyError:
        python_paths = []
    logger.info(f'pythonpath:')
    for p in python_paths:
        logger.info(f'- {p}')


import pkgutil
import sys

module_name = 'screamer'
found_locations = []


def load_module_from_path(module_name, file_path):
    # Ensure file_path points to __init__.py if it's a directory
    if os.path.isdir(file_path):
        file_path = os.path.join(file_path, module_name, "__init__.py")
        
    # Add the venv's site-packages directory temporarily to sys.path
    original_sys_path = sys.path.copy()
    sys.path.insert(0, os.path.dirname(os.path.dirname(file_path)))  # Add the venv root
    
    try:
        # Load the module as before
        spec = importlib.util.spec_from_file_location(module_name, file_path)
        logging.info('spec', spec)
        if spec and spec.loader:
            module = importlib.util.module_from_spec(spec)
            spec.loader.exec_module(module)
            return module
        else:
            raise ImportError(f"Cannot load module {module_name} from {file_path}")
    finally:
        # Restore the original sys.path
        sys.path = original_sys_path



def remove_local_screamer_path():
    while local_screamer_path in sys.path:
        sys.path.remove(local_screamer_path)

def add_local_screamer_path():
    while local_screamer_path in sys.path:
        sys.path.remove(local_screamer_path)
    sys.path.insert(0, local_screamer_path)

    
def find_local_bindings_path():
    module_name = 'screamer_bindings'
    # Locate the compiled module (.so, .pyd, or platform-specific extensions)
    possible_extensions = ['*.so', '*.pyd', '*.dll']
    for ext in possible_extensions:
        so_files = glob.glob(os.path.join(local_screamer_path, f'{module_name}{ext}'))
        if so_files:
            so_file_path = so_files[0]
            logger.info(f'found local bindings: {so_file_path}')
            return so_file_path
        
    logger.info(f'No local bindings found')
    return None

def load_local_screamer_bindings():
    module_name = 'screamer_bindings'

    local_bindings_path = find_local_bindings_path()
    if local_bindings_path is None:
        logger.info(f'No local bindings file found, unable to load then.')
        return None

    # Load the module
    spec = importlib.util.spec_from_file_location(module_name, local_bindings_path)
    logger.info(f'loading local module {module_name} from spec: {spec}')
    module = importlib.util.module_from_spec(spec)
    sys.modules[module_name] = module  # Ensure it's available globally
    spec.loader.exec_module(module)
    logger.info(f'done loading {module_name}')
    return module

def load_screamer_module_from_source():
    # First check if we can find bindings
    screamer_binding_path = find_local_bindings_path()
    if screamer_binding_path is None:
        logger.info(f"Not proceeeding to loading local screamer because we can't find local bindings")
        return None

    # Ok, now load local screamer (and in doing so the bindings)
    module_name = 'screamer'
    add_local_screamer_path() # Add the local screamer path to the start of the search
    spec = importlib.util.find_spec(module_name)
    if spec is None:
        logger.info(f'Unable to find {module_name} module locally')
        return None
    
    module = importlib.util.module_from_spec(spec)
    sys.modules[module_name] = module  # Ensure it's available globally
    spec.loader.exec_module(module)
    logger.info(f'done loading local {module_name} from spec {spec}')
    return module

def load_screamer_module_from_env():
    logger.info(f'load_screamer_module_from_env()')
    module_name = 'screamer'
    module = load_module_from_path(module_name, venv_site_packages_path)
    sys.modules[module_name] = module  # Ensure it's available globally
    return module


def load_screamer_module():
    screamer_module = load_screamer_module_from_source()
    if screamer_module is not None:
        logger.info(f'load_screamer_module has loaded LOCAL source version')
        logger.info(dir(screamer_module))
        return screamer_module
    
    screamer_module = load_screamer_module_from_env()
    if screamer_module is not None:
        logger.info(f'load_screamer_module has loaded pip installed ENV version')
        logger.info(dir(screamer_module))
        return screamer_module

    logger.info(f'load_screamer_module FAILED to load any version')
    return None


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