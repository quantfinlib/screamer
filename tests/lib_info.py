import inspect
import re
import screamer.screamer_bindings

def camel_to_snake(name):
    # Insert an underscore before an uppercase letter if preceded by a lowercase letter
    return re.sub(r'(?<=[a-z])([A-Z])', r'_\1', name).lower()


def get_constructor_arguments_from_doc(cls):
    # Get the __init__ method docstring
    init_doc = cls.__init__.__doc__
    
    if not init_doc:
        return None
    
    # Try to extract the argument list from the docstring using a regex
    signature_match = re.search(r'__init__\(self.*?\)\s*->.*', init_doc)
    
    if not signature_match:
        return None
    
    signature_line = signature_match.group(0)
    
    # Extract argument names and types from the signature line
    arg_pattern = re.compile(r'(\w+): (\w+)')
    args = arg_pattern.findall(signature_line)
    
    # Extract argument descriptions from :param sections
    param_pattern = re.compile(r':param (\w+): (.*)')
    param_descriptions = {name: desc for name, desc in param_pattern.findall(init_doc)}
    
    # Combine the arguments, types, and descriptions
    arg_info = []
    for name, arg_type in args:
        if name != 'self':  # Ignore 'self'
            arg_info.append({
                'name': name,
                'type': arg_type,
                'description': param_descriptions.get(name, 'No description available')
            })
    
    return arg_info


def collect_class_info(module):
    info = {}
    classes = inspect.getmembers(module, inspect.isclass)
    for class_name, class_obj in classes:
        info[class_name] = {}
        info[class_name]['args'] = get_constructor_arguments_from_doc(class_obj)
        info[class_name]['doc'] = class_obj.__doc__
        info[class_name]['snake_name'] = camel_to_snake(class_name)
    info = {key: info[key] for key in sorted(info)}
    return info