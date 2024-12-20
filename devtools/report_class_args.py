import inspect
import re
import sys
from devtools import get_constructor_arguments, sii

screamer_module = sii.load_screamer_module()

# Replace this with your project module name if needed.
MODULE_PATH = 'screamer'


def report_class_args(screamer_module, class_name_filter=None):
    """Print constructor arguments and types for each class in the module."""

    classes = [cls for cls_name, cls in inspect.getmembers(screamer_module, inspect.isclass)]

    for cls in classes:
        if class_name_filter and cls.__name__ != class_name_filter:
            continue
        
        constructor_args = get_constructor_arguments(cls)
        if constructor_args:
            args_formatted = ', '.join([f"{name}: {arg_type}" for name, arg_type in constructor_args])
            print(f"This is the signature of the constructor of the class {cls.__name__}:")
            print(f"{cls.__name__}( {args_formatted} )")
        else:
            print(f"{cls.__name__}() - this constructor has no arguments.")

if __name__ == "__main__":
    # Check if a class name is provided as a command-line argument
    if len(sys.argv) > 1:
        report_class_args(screamer_module, sys.argv[1])
    else:
        report_class_args(screamer_module)
