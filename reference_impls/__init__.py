import inspect
from .numpy import *
from .pandas import *
from .screamer import *
import pandas as pd

def all():

    ans = []
    # Scan all global functions
    for name, obj in globals().items():
        if callable(obj):

            # only look at the submodules
            if  '.' not in obj.__module__:
                continue

            # Check if the function belongs to the intended modules
            source_module = inspect.getmodule(obj)
            if source_module and source_module.__name__.startswith('reference_impls'):
                # Get the function signature (argument names)
                signature = inspect.signature(obj)
                args = ','.join([param.name for param in signature.parameters.values()])
                lib, func, var = (name.split('__') + ['','',''])[:3] 
                ans.append({
                    'callable': name,
                    'lib': lib, 
                    'func': func,
                    'var': var,
                    'args': args
                })
    return pd.DataFrame(ans)
