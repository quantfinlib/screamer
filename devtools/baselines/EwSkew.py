import numpy as np
import pandas as pd

def compute_partial_sum(length, a):
    w = np.zeros(length)
    w[0] = 1.0
    # Compute partial sums iteratively
    for i in range(1, length):
        w[i] = w[i - 1] * a + 1.0
    return w

def compute_n_eff(length, alpha):
    w = compute_partial_sum(length, 1-alpha)
    w2 = compute_partial_sum(length, (1-alpha)**2)
    n_eff = (w ** 2) / w2
    return n_eff

class EwSkew_pandas:
    def __init__(self, com=None, span=None, halflife=None, alpha=None):
        self.com = com
        self.span = span
        self.halflife = halflife
        self.alpha = alpha

    def __call__(self, x):
        series = pd.Series(x)
        length = len(series)

        # Determine alpha
        if self.alpha is not None:
            alpha = self.alpha
        elif self.com is not None:
            alpha = 1 / (1 + self.com)
        elif self.span is not None:
            alpha = 2 / (self.span + 1)
        elif self.halflife is not None:
            alpha = 1 - np.exp(-np.log(2) / self.halflife)
        else:
            raise ValueError("One of com, span, halflife, or alpha must be provided.")

        n_eff = compute_n_eff(length, alpha)


        # Calculate the weighted mean and standard deviation
        ewm_mean = series.ewm(com=self.com, span=self.span, halflife=self.halflife, alpha=self.alpha).mean()
        ewm_var = series.ewm(com=self.com, span=self.span, halflife=self.halflife, alpha=self.alpha).var() *  n_eff / (n_eff - 1.0)
        ewm_std = np.sqrt(ewm_var)
        
        # Compute the third central moment (m3)
        ewm_cubed_diff = ((series - ewm_mean) / ewm_std) ** 3
        m3 = ewm_cubed_diff.ewm(com=self.com, span=self.span, halflife=self.halflife, alpha=self.alpha).mean()

        # Bias correction for skewness
        g1 = m3
        skew = (n_eff * g1) / ((n_eff - 1) * (n_eff - 2))
        skew[n_eff <= 2] = np.nan  # Handle cases where n_eff is too small

        return skew.to_numpy()

"""
            double n_eff = sum_w_ * sum_w_ / sum_w2_;

            // Compute the weighted mean
            double mean = sum_x_ / sum_w_;

            // Compute the weighted variance
            double variance = (sum_xx_ / sum_w_) - (mean * mean);
            variance *= n_eff / (n_eff - 1.0);
            double std_dev = std::sqrt(variance);

            // Compute the third central moment (m3)
            double m3 = (sum_xxx_ / sum_w_) - 3 * mean * (sum_xx_ / sum_w_) + 2 * mean * mean * mean;

            // Adjust skewness using Pandas-like bias correction
            double g1 = m3 / (std_dev * std_dev * std_dev);
            double skew = (n_eff * g1) / ((n_eff - 1.0) * (n_eff - 2.0));
"""