import numpy as np
import screamer
from screamer import Lag, lag_generator
import screamer.screamer_bindings

def main():

    print('screamer library version:', screamer.__version__)

    values = [10, 20, 30, 40]
    delay = 2
    initial = 0.0

    print('The genererator version of lag')
    gen = lag_generator(values, delay, initial)
    for lagged in gen:
        print(lagged)


    # Use the transform method on a NumPy array
    lag = Lag(2, 0.0)

    arr = np.array([10, 20, 30, 40])
    result = lag.transform(arr)
    print("Original array:", arr)
    print("Lagged array:", result)  # Output will show delayed values

if __name__ == "__main__":
    main()
