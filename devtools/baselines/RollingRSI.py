import numpy as np

class EwRSI_numpy:
    def __init__(self, window_size=14):
        self.window_size = window_size

    def __call__(self, x):
        # Ensure input is a numpy array for easier element-wise operations
        x = np.asarray(x, dtype=float)
        
        # Compute the price differences
        delta = np.diff(x, prepend=x[0])
        
        # Separate gains and losses from delta
        gain = np.where(delta > 0, delta, 0)
        loss = np.where(delta < 0, -delta, 0)

        # Compute the rolling average gains and losses
        avg_gain = np.empty_like(x)
        avg_loss = np.empty_like(x)
        avg_gain[:self.window_size] = np.nan  # First period values will be NaN
        avg_loss[:self.window_size] = np.nan

        # Initialize first values for gains and losses as simple averages over the period
        avg_gain[self.window_size] = np.mean(gain[:self.window_size])
        avg_loss[self.window_size] = np.mean(loss[:self.window_size])

        # Compute the rest using the exponential moving average approach
        for i in range(self.window_size + 1, len(x)):
            avg_gain[i] = (avg_gain[i - 1] * (self.window_size - 1) + gain[i]) / self.window_size
            avg_loss[i] = (avg_loss[i - 1] * (self.window_size - 1) + loss[i]) / self.window_size

        # Calculate the RSI based on average gain and average loss
        rs = avg_gain / avg_loss
        rsi = 100 - (100 / (1 + rs))

        # Set RSI[:period] to NaN for the initial look-back period
        rsi[:self.window_size] = np.nan

        return rsi
