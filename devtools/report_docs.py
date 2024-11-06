import os
import glob
import inspect
import importlib.util
from devtools import get_module_classes, screamer_module
from colorama import Fore, Style, init

# Replace this with your project module name or dynamically load like before if needed.
MODULE_PATH = 'screamer'
CLASS_DOCS_FOLDER = 'docs'

def get_all_md_files(doc_path=CLASS_DOCS_FOLDER):
    """Collect all .md files from the docs directory and subdirectories."""
    md_files = {}
    for root, _, files in os.walk(doc_path):
        for file in files:
            if file.endswith('.md'):
                class_name = os.path.splitext(file)[0]
                md_files[class_name] = os.path.join(root, file)
    return md_files

def get_all_classes_in_module(module_path=MODULE_PATH):
    """Collect all class names from the compiled module."""
    so_file_path = glob.glob(f'{module_path}/{module_path}_bindings*.so')[0]
    spec = importlib.util.spec_from_file_location(f'{module_path}_bindings', so_file_path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    
    classes = [name for name, obj in inspect.getmembers(module, inspect.isclass)]
    return classes


def report_class_documentation(screamer_module):

    # Initialize colorama
    init(autoreset=True)
    
    classes = get_module_classes(screamer_module)
    md_files = get_all_md_files()

    good_count = 0
    bad_count = 0
    for class_name in classes:
        if class_name.startswith('_'):
            continue
        if class_name in md_files:
            print(f"{Fore.GREEN}OK {class_name:20s} -> {md_files[class_name]}")
            good_count += 1
        else:
            print(f"{Fore.RED}{Style.BRIGHT}!! {class_name:20s} Missing Docs!")
            bad_count += 1  # Fixed incrementing `bad_count`

    # Print overview of successes and errors
    print("\nSummary:")
    print(f"{Fore.GREEN}Total OK: {good_count}")
    if bad_count > 0:
        print(f"{Fore.RED}Total Missing Docs: {bad_count}")
    else:
        print(f"{Fore.GREEN}Total Missing Docs: {bad_count}")

if __name__ == "__main__":
    
    report_class_documentation(screamer_module)
