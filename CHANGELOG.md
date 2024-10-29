# Changelog

All notable changes to this project are documented in this file.
 
[Unreleased] - yyyy-mm-dd
-------------------------

### Added

#### Data handeling

* fillna
* ffill
* clip

#### Math

* Abs
* Sign
* Exp
* Log
* Sqrt
* Erf
* Erfc

#### Simple transforms

* Diff
* Lag
* Return 
* LogReturn

#### Rolling window functions

* rolling std
* rolling skew
* rolling kurtosis
* rolling zscore
* rolling min
* rolling max
* rolling median
* rolling quantile
* rolling rms

#### Exponentiually weighted functions

* EwMean
* EwStd
* EwVar
* EwSkew
* EwKurt
* EwRms
  
#### Interface
* support for iterator / generator processing

### Fixed
* Fixed incorrect results when applying transforms to a view on a numpy array.

### Changed
* removed the transform member functions

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


