# screamer/generators.py

from .screamer_bindings import Lag, Diff, RollingSum, Return, LogReturn, SMA


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


def rolling_sum_generator(iterable, window_size):
    """
    Generator that yields the rolling sum  from the input iterable.

    :param iterable: An iterable of numerical values.
    :param window_size: The window size of the rolling sum.
    """
    trans = RollingSum(window_size)
    for value in iterable:
        yield trans(value)  


def return_generator(iterable, delay):
    """
    Generator that yields simple return values from the input iterable.

    :param iterable: An iterable of numerical values.
    :param delay: Number of steps to lag in the return calculation.
    """
    trans = Return(delay)
    for value in iterable:
        yield trans(value)


def log_return_generator(iterable, delay):
    """
    Generator that yields log return values from the input iterable.

    :param iterable: An iterable of numerical values.
    :param delay: Number of steps to lag in the log return calculation.
    """
    trans = LogReturn(delay)
    for value in iterable:
        yield trans(value)

def sma_generator(iterable, window_size):
    """
    Generator that yields the simple moving average from the input iterable.

    :param iterable: An iterable of numerical values.
    :param window_size: The window size of the simple moving average.
    """
    trans = SMA(window_size)
    for value in iterable:
        yield trans(value)  
