# Changelog

All notable changes to this project are documented in this file.
 
[Unreleased] - yyyy-mm-dd
-------------------------

### Added

* exponentially weighted moving averge
* rolling std
* rolling skew
* rolling kurtosis
* rolling zscore

### Fixed

* Fixed incorrect results when applying transforms to a view on a numpy array.

Version v0.1.32 (2024-10-20)
-------------------------

### Added

* Differences, Simple Return, Log Return, Rolling Sum, Simple Moving Average

### Changed
* removed initial_value from the constructor, we (for now) return NaN values when we cant resolve indicators.

Version v0.1.31 (2024-10-20)
-------------------------

### Added
* The indicator.transform() member functions can now transform multi-dimensional numpy arrays.
* Added documentation.


