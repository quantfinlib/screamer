import inspect
import screamer.screamer_bindings

# List all classes in the module
def list_classes_and_docs(module):
    # Get all the classes in the module
    classes = inspect.getmembers(module, inspect.isclass)
    
    for class_name, class_obj in classes:
        # Print the class name
        print(f"Class: {class_name}")

        # Get and print the docstring (if available)
        docstring = class_obj.__doc__ or "No docstring available"
        print(f"Description: {docstring}\n")

        #print(inspect.signature(class_obj.__init__))
        print(class_obj.__init__.__doc__)


# Call the function to list classes and their docstrings
list_classes_and_docs(screamer.screamer_bindings)
