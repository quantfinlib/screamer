# Changelog

All notable changes to this project are documented in this file.
 
[Unreleased] - yyyy-mm-dd
-------------------------

Version v0.1.44 (2024-11-01)
-------------------------

Version v0.1.43 (2024-11-01)
-------------------------

Version v0.1.42 (2024-11-01)
-------------------------

Version v0.1.41 (2024-11-01)
-------------------------

Version v0.1.40 (2024-11-01)
-------------------------

Version v0.1.39 (2024-11-01)
-------------------------

Version v0.1.38 (2024-11-01)
-------------------------

Version v0.1.37 (2024-11-01)
-------------------------

Version v0.1.36 (2024-11-01)
-------------------------

Version v0.1.35 (2024-11-01)
-------------------------

* Improved documentation

Version v0.1.34 (2024-10-31)
-------------------------

Version v0.1.33 (2024-10-31)
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
* rolling poly1, 1rst order polynomial fit
* rolling poly2, 2nd order polynomial fit

#### Exponentiually weighted functions

* EwMean
* EwStd
* EwVar
* EwSkew
* EwKurt
* EwRms
  
#### Filters

* 2nd order Butterworth


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


