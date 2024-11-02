class Linear_numpy_v1:
    def __init__(self, scale, shift):
        self.scale = scale
        self.shift = shift

    def __call__(self, x):
        import numpy as np
        return self.scale * np.array(x) + self.shift

class Linear_numpy_v2:
    def __init__(self, scale, shift):
        self.scale = scale
        self.shift = shift

    def __call__(self, x):
        import numpy as np
        return np.multiply(self.scale, x) + self.shift

class Linear_pandas:
    def __init__(self, scale, shift):
        self.scale = scale
        self.shift = shift

    def __call__(self, x):
        import pandas as pd
        return (self.scale * pd.Series(x) + self.shift).to_numpy()
