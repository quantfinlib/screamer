# screamer/generators.py

from .screamer_bindings import Lag, Diff

def lag_generator(iterable, delay):
    """
    Generator that yields lagged values from the input iterable.

    :param iterable: An iterable of numerical values.
    :param delay: Number of steps to lag.
    """
    lag = Lag(delay)
    for value in iterable:
        yield lag(value)

def diff_generator(iterable, delay):
    """
    Generator that yields changed values from the input iterable.

    :param iterable: An iterable of numerical values.
    :param delay: Number of steps to lag.
    """
    diff = Diff(delay)
    for value in iterable:
        yield diff(value)