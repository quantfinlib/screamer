
# Making a Release

We thave `invoke` tasks in `tasks.py` for making releases. Use one of the following:

```
invoke release --part patch
invoke release --part minor
invoke release --part major
```


# Local Testing
 
Local testing compiles the C++ code,  re-installs the library, and runs pytest:

 ```
 invoke test
 ```

# Reference implemenations

After adding a class `ABC` to screamer you should add a function `screamer__abc(array, ...)`
to `reference_impls/screamer.py` that call this function. 

Additionally you should aim to add an independent reference implemenation and call it
 `<other>_abc(array, ..)`. Currently we have collected reference implemenation based on 
 numpy and pandas in the `reference_impls/numpy.py` and `reference_impls/pandas.py` modules.

The general function name convention is:
 `<reference name>__<function name>[__<optional variant name>]`. E.g. the 
function `numpy__rolling_mean__cumsum` is a `numpy` implementation of the `rolling_mean` 
function, and this specific implementation variant uses `cumsum`.  Specifying an implementation 
variant name is optional, it allows for implementing multiple variant of the same algorithm
using the same basis library (`numpy` in this case)

 During testing the script `test/test_reference_impl.py` will scan these implementations and 
 run and compare them against eachother.

The `reference_impls` modules also container an `all()` function that returns a dynamically 
assembled pandas DataFrame of all the functions and reference implementation. It looks 
like this:

|           callable             |    lib    |        func         |    var    |              args               |
|---------------------------------|-----------|---------------------|----------|---------------------------------|
| numpy__rolling_mean__cumsum     | numpy     | rolling_mean        | cumsum   | array,window_size               |
| numpy__rolling_mean__stride     | numpy     | rolling_mean        | stride   | array,window_size               |
| numpy__rolling_std__stride      | numpy     | rolling_std         | stride   | array,window_size               |
| numpy__rolling_skew__stride     | numpy     | rolling_skew        | stride   | array,window_size               |
| numpy__rolling_kurt__stride     | numpy     | rolling_kurt        | stride   | array,window_size               |
| numpy__rolling_min__stride      | numpy     | rolling_min         | stride   | array,window_size               |
| numpy__rolling_max__stride      | numpy     | rolling_max         | stride   | array,window_size               |
| numpy__rolling_median__stride   | numpy     | rolling_median      | stride   | array,window_size               |
| numpy__rolling_quantile__stride | numpy     | rolling_quantile    | stride   | array,window_size,quantile      |
| numpy__diff                     | numpy     | diff                |          | array,window_size               |
| numpy__lag                      | numpy     | lag                 |          | array,window_size               |
| pandas__rolling_mean            | pandas    | rolling_mean        |          | array,window_size               |
| pandas__rolling_std             | pandas    | rolling_std         |          | array,window_size               |
| pandas__rolling_skew            | pandas    | rolling_skew        |          | array,window_size               |
| pandas__rolling_kurt            | pandas    | rolling_kurt        |          | array,window_size               |
| pandas__rolling_min             | pandas    | rolling_min         |          | array,window_size               |
| pandas__rolling_max             | pandas    | rolling_max         |          | array,window_size               |
| pandas__rolling_median          | pandas    | rolling_median      |          | array,window_size               |
| pandas__rolling_quantile        | pandas    | rolling_quantile    |          | array,window_size,quantile      |
| screamer__rolling_mean          | screamer  | rolling_mean        |          | array,window_size               |
| screamer__rolling_var           | screamer  | rolling_var         |          | array,window_size               |
| screamer__rolling_std           | screamer  | rolling_std         |          | array,window_size               |
| screamer__rolling_skew          | screamer  | rolling_skew        |          | array,window_size               |
| screamer__rolling_kurt          | screamer  | rolling_kurt        |          | array,window_size               |
| screamer__rolling_min           | screamer  | rolling_min         |          | array,window_size               |
| screamer__rolling_max           | screamer  | rolling_max         |          | array,window_size               |
| screamer__rolling_median        | screamer  | rolling_median      |          | array,window_size               |
| screamer__diff                  | screamer  | diff                |          | array,window_size               |
| screamer__lag                   | screamer  | lag                 |          | array,window_size               |


This table is basis of comparing screamer functions against reference implementations,
both for unit testing and for performance benchmark tests.

The  `test/test_reference_impl.py` script collect this table, loop over all function in the 
screamer lib, and looks for the same function in other libs. If a pair is found it will 
run a test to see if both version give the same return value.


# Running Examples

```
poetry run python examples/lag_example.py
```

# Github Actions

## ci.yml

This action is triggered on any commit to main. It uses `cibuildwheel` to build the 
Python 3.11 wheel for the Linux platform, installs it in a clean isolated environment,
 and finally runs pytest.

 ## publish.yml

 This action is triggered by a version tag.  It build Python 3.9, 3.10 and 3.11 wheel
 for Windows, Linux and OSX, runs pytest, and if all succeeds published a new version
 pn pypi.
 