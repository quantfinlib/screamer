Usage
=====

This section provides examples demonstrating different ways to use `screamer` functions. 
All `screamer` functions operate similarly, with slight variations based on the specific settings
or each function. For instance, rolling functions require a `window_size` parameter,
while `RollingPoly` functions additionally include a `derivative_order` parameter.

Using `screamer` functions generally involves two main steps. First, create a `screamer` 
function object with the desired parameters. Once initialized, you can pass either streaming 
or batch data to this function object, which will then process each data element according to
its defined behavior. The `screamer` library supports both batch and streaming data processing, 
enabling smooth transitions between backtesting transformation on a training dataset, and puting
it live in production to run on streaming data-feeds.



Batch-processing numpy arrays
-----------------------------

This example demonstrates the simplest usage of `screamer` for batch-processing 
a NumPy array with a `RollingMax` object. Given a one-dimensional array,
`RollingMax` calculates the rolling maximum for each element over a window 
size of 4. You can achieve this by either instantiating the object and applying 
it to the array or by using a single-line approach where `RollingMax(window_size=4)(data)`
directly computes the result. This approach is efficient for batch-processing pre-loaded
data and allows you to apply rolling calculations effortlessly, with results immediately
available as a new array.

.. exec_code::
   :linenos:

   # --- hide: start ---
   import numpy as np
   from screamer import RollingMax

   np.random.seed(42)
   # --- hide: stop ---
   # Sample data
   data = np.random.normal(size=10) 

   # --- hide: start ---
   print("data:")
   print(data)

   # --- hide: stop ---

   # Create a screamer object and pass the numpy array
   obj = RollingMax(window_size=4)
   result = obj(data)

   # or you can do it in a 1-lines
   result = RollingMax(window_size=4)(data)
   # --- hide: start ---

   print("Result:")
   print(result)
   # --- hide: stop ---


Batch-processing multi-dimensional numpy arrays
-----------------------------------------------

This example demonstrates batch processing of multi-dimensional NumPy arrays, 
such as matrices, using a `screamer` object. Here, the `RollingMax` class is 
applied with a window size of 4 to each column independently. When a 
multi-dimensional array is passed to `RollingMax`, the class processes each 
column separately, applying the rolling maximum operation column-wise rather 
than across the entire matrix. This allows each column’s sequence of values 
to retain individual transformations, making it easy to perform parallel or 
grouped calculations on structured data without reshaping or flattening. 
This approach is particularly useful when working with time series data across
multiple sensors or features, where each feature is processed independently 
but in a synchronized manner.


.. exec_code::
   :linenos:

   # --- hide: start ---
   import numpy as np
   from screamer import RollingMax

   np.random.seed(42)
   # --- hide: stop ---
   # Sample data
   data = np.random.normal(size=(10, 3)) 

   # --- hide: start ---
   print("data:")
   print(data)

   # --- hide: stop ---

   # Create a screamer object and pass the numpy array
   result = RollingMax(window_size=4)(data)
   # --- hide: start ---

   print("Result:")
   print(result)
   # --- hide: stop ---


Stream processing one element at a time
---------------------------------------

This example demonstrates using a `screamer` object as an element-wise processor. Here, 
the `RollingMax` class is created for computing a rolling maximum with a window size of 4.
 
By using the `RollingMax` object directly on an element-by-element basis, each value is processed 
in sequence, allowing efficient, element-by-element transformation without 
requiring the entire dataset to be loaded into memory. This setup is particularly 
suited for streaming data applications.

.. exec_code::
   :linenos:

   # --- hide: start ---
   import numpy as np
   from screamer import RollingMax

   np.random.seed(42)
   # --- hide: stop ---
   # Sample data
   data = np.random.normal(size=10) 

   # --- hide: start ---
   print("data:")
   print(data)

   # --- hide: stop ---

   # Create a screamer object and pass one element at a time
   obj = RollingMax(window_size=4)
   for x in data:
      print(f'input: {x:.4f}, output: {obj(x):.4f}')



Screamer objects as generators
------------------------------

This example illustrates how to use a `screamer` object, specifically `RollingMax`, 
as a generator for real-time data transformation. In this setup, `RollingMax` is 
configured with a window size of 4, creating a generator that calculates the rolling
maximum as each element is processed. By treating the data as an iterable (using a 
generator), this approach processes values sequentially, making it ideal for streaming
or large datasets where loading everything into memory is impractical. Each output
value represents the rolling maximum up to that point, allowing efficient real-time 
analysis on continuous data.

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

   # --- hide: start ---
   print("data:")
   print(data)

   # --- hide: stop ---

   # Create a screamer generator that transforms the data
   obj = RollingMax(window_size=4)

   rolling_max_generator = obj(data_generator)

   for x in rolling_max_generator:
      print(f'{x:.4f}')



Generator composition
---------------------

This example demonstrates the composition of two streaming operations 
using the `screamer` library. First, a generator applies a 3-point difference 
operation on a sequence of random data, using the `Diff` class. The resulting 
generator is then passed through the `RollingMax` class, which computes the 
rolling maximum with a window size of 4. By chaining these two generators, 
we process each data element sequentially without loading the entire sequence 
into memory, making it efficient for streaming applications.

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

   # --- hide: start ---
   print("data:")
   print(data)

   # --- hide: stop ---

   # Create two screamer generators and chain them together
   obj1 = Diff(3) 
   obj2 = RollingMax(window_size=4)

   chained_generator = obj2(obj1(data_generator))

   for x in chained_generator:
      print(f'{x:.4f}')

