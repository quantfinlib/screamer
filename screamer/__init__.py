# screamer/__init__.py

from .screamer_bindings import Lag, Diff, RollingSum, Return, LogReturn, SMA
from .generators import lag_generator, diff_generator, rolling_sum_generator, return_generator, log_return_generator, sma_generator

__version__ = "0.1.32"

__all__ = [
    "Lag", "lag_generator", 
    "Diff", "diff_generator", 
    "RollingSum", "rolling_sum_generator",
    "Return", "return_generator",
    "LogReturn", "log_return_generator",
    "SMA", "sma_generator"
    ]
