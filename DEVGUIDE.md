
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
 