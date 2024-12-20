import logging
logger = logging.getLogger(__name__)
logging.basicConfig(level=logging.INFO)

import sys
import glob
import importlib.util
import inspect
import re
import os
import importlib
import devtools.baselines


class ScreamerInstallInfo:
    def __init__(self):
        self.local_project_path = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
        self.local_screamer_path = os.path.join(self.local_project_path, 'screamer')
        self.local_bindings_file_path = None
        self.env_screamer_paths = None
        self.module = None
        self._set_local_bindings_file_path()
        self._set_env_screamer_paths()

    def _set_local_bindings_file_path(self):
        self.local_bindings_path = None
        possible_extensions = ['*.so', '*.pyd', '*.dll']
        for ext in possible_extensions:
            so_files = glob.glob(os.path.join(self.local_screamer_path, f'screamer_bindings{ext}'))
            if so_files:
                logger.debug(f'get_local_bindings_path: FOUND {so_files[0]}')
                self.local_bindings_file_path = so_files[0]
                return
        logger.debug(f'get_local_bindings_path: NO BINDINGS FOUND')
        return None

    def _set_env_screamer_paths(self):
        self.env_screamer_paths = []
        pattern = r".*?site-packages"
        for path in sys.path:
            logger.debug(f'get_env_screamer_paths: EXAMINING {path}')
            match = re.search(pattern, path)
            if match:
                env_path = match.group()
                logger.debug(f'get_env_screamer_paths: MATCHED {env_path}')
                env_screamer_path = os.path.join(env_path, 'screamer')
                if os.path.isdir(env_screamer_path):
                    logger.debug(f'get_env_screamer_paths: FOUND {env_screamer_path}')
                    self.env_screamer_paths.append(env_screamer_path)

    def _load_module_from_file_path(self, module_name, file_path):
        original_sys_path = sys.path.copy()
        sys.path = [os.path.dirname(os.path.dirname(file_path))]
        logger.debug(f'loading {module_name}, changed sys.path to {sys.path}')
        spec = importlib.util.spec_from_file_location(module_name, file_path)
        logger.debug(f'loading... spec = {spec}')
        self.module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(self.module)
        sys.path = original_sys_path
        return self.module
       
    def load_local_screamer_binding(self):
        return self._load_module_from_file_path('screamer_bindings', self.local_bindings_file_path)
    
    def load_local_screamer(self):
        file_path = os.path.join(self.local_screamer_path, "__init__.py")
        return self._load_module_from_file_path('screamer', file_path)
    
    def load_env_screamer(self):
        file_path = os.path.join(self.env_screamer_paths[0], "__init__.py")
        return self._load_module_from_file_path('screamer', file_path)


    def load_screamer_module(self):
        logger.info(f'load_module:')
        if self.local_bindings_file_path:
            return self.load_local_screamer()
        
        if self.env_screamer_paths:
            logger.info(f'load_module: trying env')
            return self.load_env_screamer()

        logger.info(f'load_module: NOT LOADED, nothing tried')

sii = ScreamerInstallInfo()

def get_module_classes(screamer_module):
    class_names = [name for name, obj in inspect.getmembers(screamer_module, inspect.isclass)]
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