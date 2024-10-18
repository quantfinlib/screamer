# screamer/generators.py

from .screamer_bindings import Lag

def lag_generator(iterable, delay, initial=0.0):
    """
    Generator that yields lagged values from the input iterable.

    :param iterable: An iterable of numerical values.
    :param delay: Number of steps to lag.
    :param initial: Initial value(s) to yield before lagged values are available.
    """
    lag = Lag(delay, initial)
    for value in iterable:
        yield lag(value)
