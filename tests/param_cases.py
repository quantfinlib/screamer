from itertools import product
import numpy as np
from devtools import load_screamer_module, get_constructor_arguments, get_baselines

# ----------------------------------------------------------------------
# Set of classes in the screamer module
# ----------------------------------------------------------------------

# Load the screamer module
screamer_module = load_screamer_module()

# List of all screamer class names
screamer_classes = [cls for cls in dir(screamer_module) if not cls.startswith('_')]

# The Rolling classes, except 'RollingQuantile' which has an extra argument
rolling_classes = [cls for cls in screamer_classes if cls.startswith('Rolling') and not cls=='RollingQuantile' and not cls=='RollingFracDiff']

# The Ew classes, except: todo baselines for 'EwSkew', 'EwKurt'
ew_classes = [cls for cls in screamer_classes if cls.startswith('Ew') and not cls in['EwSkew', 'EwKurt']]

# Classes that have no arguments
no_arg_classes = [
    cls for cls in screamer_classes 
    if len(get_constructor_arguments(getattr(screamer_module, cls)))==0
]

# ----------------------------------------------------------------------
# Combination test cases (Cartesian products of parameters)
# input arrays default to: "array_length": [100], "array_type": ["default"]
# ----------------------------------------------------------------------

test_definitions = [
    { tuple(rolling_classes)     : {"window_size": [20]} },
    { tuple(['RollingQuantile']) : {"window_size": [20], "quantile": [0, 0.01, 0.4, 1]} },
    { tuple(['RollingPoly1'])    : {"window_size": [20], "derivative_order": [0, 1] }},
    { tuple(['RollingPoly2'])    : {"window_size": [20], "derivative_order": [0, 1, 2] }},
    { tuple(['RollingFracDiff']) : {"window_size": [20], "frac_order": [0.25, 0.5, 0.75, 1.0]}},       
    { tuple(ew_classes)          : {"span": [5]}},
    { tuple(ew_classes)          : {"alpha": [0.2]}},
    { tuple(ew_classes)          : {"halflife": [10]}},
    { tuple(ew_classes)          : {"com": [10]}},
    { tuple(no_arg_classes)      : {"array_type": ["positive"]}}
]

# ----------------------------------------------------------------------
# Define functions for generating special input arrays
# ----------------------------------------------------------------------

def generate_positive_array(array_length):
    """Generate an array of positive values."""
    return np.random.uniform(0.1, 10, array_length)

def generate_array_with_nan(array_length):
    """Generate an array with NaN values interspersed."""
    array = np.random.randn(array_length)
    array[::10] = np.nan  # Insert NaN every 10 elements for testing
    return array

def generate_default_array(array_length):
    """Generate a standard array with random normal values."""
    return np.random.randn(array_length)

def generate_array(array_type, array_length, **kwargs):
    if array_type == 'positive':
        return generate_positive_array(array_length)
    if array_type == 'with_nan':
        return generate_array_with_nan(array_length)
    return generate_default_array(array_length)

# ----------------------------------------------------------------------
# Generate parameter combinations
# ----------------------------------------------------------------------
def generate_combinations(param_dict):
    keys = param_dict.keys()
    values = product(*param_dict.values())
    return [dict(zip(keys, v)) for v in values]


def expanded_test_cases(test_definitions):
    """Generate and return all individual test cases as a dictionary with class groups as keys."""
    all_test_cases = {}

    for case in test_definitions:
        for class_group, params in case.items():

            # Generate Cartesian products of parameter combinations
            keys = params.keys()
            values = product(*params.values())

            combination_list = [dict(zip(keys, combination)) for combination in values]

            # Store the combinations in the dictionary under the class group
            all_test_cases[class_group] = combination_list

    return all_test_cases


# Collect all test cases for pytest parameterization
def yield_test_cases():
    """Yield tuples of (class_name, params_dict, array_type, array_length) for testing."""
    for classes_list, params_list in expanded_test_cases(test_definitions).items():
        for class_name, params_dict in product(classes_list, params_list):

            # Copy parameters and extract array properties
            params = params_dict.copy()
            array_length = params.pop("array_length", 100)
            array_type = params.pop("array_type", "default")

            yield (class_name, params, array_type, array_length)


def yield_test_cases_with_baselines():
    """Yield tuples of (class_name, baseline_name, params_dict, array_type, array_length) for testing."""
    for classes_list, params_list in expanded_test_cases(test_definitions).items():
        for class_name, params_dict in product(classes_list, params_list):
            # Collect baselines for the class
            baselines_for_class = get_baselines(class_name)
            if not baselines_for_class:
                continue  # Skip if no baselines available

            # Copy parameters and extract array properties
            params = params_dict.copy()
            array_length = params.pop("array_length", 100)
            array_type = params.pop("array_type", "default")

            for baseline_name in baselines_for_class:
                yield (class_name, baseline_name, params, array_type, array_length)
