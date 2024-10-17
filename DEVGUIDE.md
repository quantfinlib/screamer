
# Testing
 
 ## Test C++ code

* We use Google Test for the C++ unit tests. 
* We use CMake for compiling the C++ code.

 When compiling an building binaries we advice to  use  "out-of-source builds" strategy:

First create a `build` directory and run cmake from within that directory. Tell cmake to look for its configuration file
`CMakeList.txt` in the parent folder by proving the `..` argument to cmake. 

```
# From project root

rm -rf build
mkdir build
cd build
cmake ..
```

Then every time you have updated code, you can build the exedcutables and run ctest:

```
# From project root
cd build
cmake --build .
ctest --output-on-failure
```

## Testing Python interface

For testing the Python interface we use `pytest`:
```
# From project root

poetry run pytest tests/test_lag.py
```


# Running Examples

```
poetry run python examples/lag_example.py
```
