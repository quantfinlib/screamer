## Using `screamer` Functions

`screamer` functions share a common structure with function-specific settings. 

1. First, create a `screamer` function object by specifying the parameters for data transformation. For example, `obj = RollingMean(30)` creates an object to apply a 30-step moving average to the input data.

2. Next, pass either streaming data (`obj(datafeed)`) or batch data (`obj(dataset)`) to this object. It will then process each data element as specified.

3. `screamer` functions return data in the same format as the input. A NumPy array input returns a NumPy array of the same size, while a Python generator input returns a generator that yields transformed elements.

The `screamer` library’s unified interface supports both batch and streaming data, making it easy to transition from backtesting on historical datasets to live streaming deployment without modifying your code.

---

### Batch Processing with NumPy Arrays

This example shows the simplest use of `screamer` for batch processing a NumPy array with `RollingMax`. Given a one-dimensional array, `RollingMax` computes the rolling maximum over a specified window size, here set to 4. You can initialize the object once and apply it, or perform this in a single line with `RollingMax(window_size=4)(data)`. This approach efficiently processes pre-loaded data with results returned as a new array.

```{eval-rst}
.. exec_code::
   :linenos:

   # --- hide: start ---
   import numpy as np
   from screamer import RollingMax

   np.random.seed(42)
   # --- hide: stop ---
   # Sample data
   data = np.random.normal(size=10) 

   # Create a screamer object and pass the numpy array
   obj = RollingMax(window_size=4)
   result = obj(data)

   # or in one line
   result = RollingMax(window_size=4)(data)
   # --- hide: start ---
   print("Result:")
   print(result)
```

---

### Batch Processing Multi-Dimensional Arrays

For multi-dimensional arrays like matrices, `screamer` applies rolling operations column-by-column. Using `RollingMax` with a window size of 4, this example processes each column independently. This method is particularly useful for time series data with multiple features, where each column (representing a feature) is transformed independently but remains synchronized across rows.

```{eval-rst}
.. exec_code::
   :linenos:

   # --- hide: start ---
   import numpy as np
   from screamer import RollingMax

   np.random.seed(42)
   # --- hide: stop ---
   # Sample data
   data = np.random.normal(size=(10, 3)) 

   # Process the multi-dimensional array with RollingMax
   result = RollingMax(window_size=4)(data)
   # --- hide: start ---
   print("Result:")
   print(result)
```

---

### Element-Wise Stream Processing

`screamer` functions support element-by-element processing for streaming applications. In this example, `RollingMax` calculates the rolling maximum for each new element as it arrives, using a window size of 4. Processing data sequentially enables real-time transformation without loading the entire dataset into memory, ideal for streaming use cases.

```{eval-rst}
.. exec_code::
   :linenos:

   # --- hide: start ---
   import numpy as np
   from screamer import RollingMax

   np.random.seed(42)
   # --- hide: stop ---
   # Sample data
   data = np.random.normal(size=10) 

   # Element-by-element processing
   obj = RollingMax(window_size=4)
   for x in data:
      print(f'input: {x:.4f}, output: {obj(x):.4f}')
```

---

### Using `screamer` Objects as Generators

`screamer` objects can act as generators for real-time data transformation. This example sets up `RollingMax` as a generator with a 4-element window, allowing it to compute the rolling maximum as elements arrive. This approach processes data as an iterable, suitable for large or streaming datasets where loading everything at once is impractical.

```{eval-rst}
.. exec_code::
   :linenos:

   # --- hide: start ---
   import numpy as np
   from screamer import RollingMax

   np.random.seed(42)
   # --- hide: stop ---
   # Sample data
   data = np.random.normal(size=10) 
   data_generator = iter(data)

   # RollingMax as a generator
   obj = RollingMax(window_size=4)
   rolling_max_generator = obj(data_generator)

   for x in rolling_max_generator:
      print(f'{x:.4f}')
```

---

### Composing Generators

`screamer` supports chaining multiple generators for complex transformations. Here, `Diff` first applies a 3-point difference operation, and `RollingMax` then computes the rolling maximum on the results with a window size of 4. This composition processes each data element sequentially, making it efficient for streaming applications where memory conservation is key.

```{eval-rst}
.. exec_code::
   :linenos:

   # --- hide: start ---
   import numpy as np
   from screamer import RollingMax, Diff

   np.random.seed(42)
   # --- hide: stop ---
   # Sample data
   data = np.random.normal(size=10) 
   data_generator = iter(data)

   # Compose Diff and RollingMax as chained generators
   diff = Diff(3) 
   rmax = RollingMax(window_size=4)
   chained_generator = rmax(diff(data_generator))

   for x in chained_generator:
      print(f'{x:.4f}')
```
