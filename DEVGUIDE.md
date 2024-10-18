
# Making a releaser

We thave `invoke` tasks in `tasks.py` for mkaing releases. Some invoke commands are:

```
invoke release --part patch
invoke release --part minor
invoke release --part major
```


# Testing
 
 ## Test C++ code

 ```
 invoke ctest
 ```

This task bascially does:
```
# From project root

rm -rf build
mkdir build
cd build
cmake ..
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
