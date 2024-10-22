# DO NOT MODIFY, THIS FILE IS AUTOGENERATED
# screamer/generators.py

from .screamer_bindings import (
    Diff,
    Lag,
    Lagq,
    LogReturn,
    Return,
    RollingSum,
    SMA,
    SMAL             
)

def diff_generator(iterable, delay):
    """
    Generator version of the Diff class.

    :param iterable: An iterable of numerical values.
    :param delay: The delay parameter, d.    
    """
    indicator = Diff(delay)
    for value in iterable:
        yield indicator(value)


def lag_generator(iterable, delay):
    """
    Generator version of the Lag class.

    :param iterable: An iterable of numerical values.
    :param delay: The delay parameter, d.    
    """
    indicator = Lag(delay)
    for value in iterable:
        yield indicator(value)


def lagq_generator(iterable, delay):
    """
    Generator version of the Lagq class.

    :param iterable: An iterable of numerical values.
    :param delay: The delay parameter, d.    
    """
    indicator = Lagq(delay)
    for value in iterable:
        yield indicator(value)


def log_return_generator(iterable, delay):
    """
    Generator version of the LogReturn class.

    :param iterable: An iterable of numerical values.
    :param delay: No description available    
    """
    indicator = LogReturn(delay)
    for value in iterable:
        yield indicator(value)


def return_generator(iterable, delay):
    """
    Generator version of the Return class.

    :param iterable: An iterable of numerical values.
    :param delay: No description available    
    """
    indicator = Return(delay)
    for value in iterable:
        yield indicator(value)


def rolling_sum_generator(iterable, window_size):
    """
    Generator version of the RollingSum class.

    :param iterable: An iterable of numerical values.
    :param window_size: The window_size parameter.    
    """
    indicator = RollingSum(window_size)
    for value in iterable:
        yield indicator(value)


def sma_generator(iterable, window_size):
    """
    Generator version of the SMA class.

    :param iterable: An iterable of numerical values.
    :param window_size: The window_size parameter.    
    """
    indicator = SMA(window_size)
    for value in iterable:
        yield indicator(value)


def smal_generator(iterable, window_size):
    """
    Generator version of the SMAL class.

    :param iterable: An iterable of numerical values.
    :param window_size: The window_size parameter.    
    """
    indicator = SMAL(window_size)
    for value in iterable:
        yield indicator(value)

