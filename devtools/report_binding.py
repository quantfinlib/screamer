import os
import re
import sys

BINDINGS_FOLDER = 'bindings'

def get_binding_info_for_class(class_name):
    """Extract binding information for a specific class from C++ binding files."""
    bindings_info = []
    
    # Regex pattern to match the class binding entry
    class_pattern = re.compile(
        rf'py::class_<\s*screamer::{class_name}.*?\)\s*\.def\(.*?\);',
        re.DOTALL
    )
    
    # Scan through all C++ files in the bindings folder
    for root, _, files in os.walk(BINDINGS_FOLDER):
        for file in files:
            if file.endswith('.cpp'):
                file_path = os.path.join(root, file)
                with open(file_path, 'r', encoding='utf-8') as f:
                    content = f.read()
                    
                    # Search for binding entries in the file
                    matches = class_pattern.findall(content)
                    if matches:
                        for match in matches:
                            bindings_info.append((file_path, match))
    
    return bindings_info

def print_binding_info(class_name):
    """Print binding information for the specified class."""
    bindings_info = get_binding_info_for_class(class_name)
    
    if bindings_info:
        for file_path, binding in bindings_info:
            print(f"This is the binding info for '{class_name}' that in located in the file {file_path}:\n")
            print(binding)
    else:
        print(f"No binding information found for class '{class_name}'.")

if __name__ == "__main__":
    if len(sys.argv) > 1:
        class_name = sys.argv[1]
        print_binding_info(class_name)
    else:
        print("Usage: python extract_bindings_info.py <ClassName>")
