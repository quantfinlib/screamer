import os
import importlib

# Automatically import all Python modules in this directory
for filename in os.listdir(os.path.dirname(__file__)):
    if filename.endswith(".py") and filename != "__init__.py":
        module_name = filename[:-3]  # Remove the '.py' extension
        module = importlib.import_module(f".{module_name}", package=__name__)
        globals().update({name: getattr(module, name) for name in dir(module) if isinstance(getattr(module, name), type)})
