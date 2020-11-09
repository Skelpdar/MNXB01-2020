# MNXB01-2020
Analyze temperature data with ROOT, branching into two separate sub-projects.

Tested in ROOT 6.14.04, but this doesn't depend on any recently deprecated or brand new functionality.
It should therefore be unproblematic to run on any reasonbly up-to-date ROOT installation. 

### Instructions
The data pre-processing is done as follows:
cd to the folder where the data is to be placed (in data/ here), then call upon smhicleaner.sh. This shell-script takes the path of the raw data (in rawdata/) as its first argument. 

## Temperature Interpolation
Author: Erik

A map of Sweden is available in map.png. We have a few sprase measurement weather stations dotted around Sweden in rawdata/

maps.C can interpolate temperature over the rest of the map using various methods, as well as drawing it as a ROOT histogram. 

### Instructions
In the ROOT-prompt, compile the data loading help-script:
```
.L dataLoader.C+
```
then run the macro directly in the interpreter:
```
.L maps.C
```
The maps() function takes as its first argument an integer between 0 and 3, with the following options:

0: Interpolate temperature over the whole map, with a linear fit.
1: Interpolate the temperature, by taking taking the temperature at the nearest measurement.
2: Interpolate by taking the weighted average of the two nearest measurements.
3: Intepolate by making a linear fit on the three nearest measurements. 

## Fun Facts
Author: Peter

### Instructions
To run the code:
Compile with 
```
.L funFacts.C+
```
Run 
```
funfact()
```
Type a date in the form YYYY-MM-DD (between 2010 and 2019)

### Fun Facts Changelog
10-30: devised method the use time values as DTime in x[]

11-01: Created ability to identify hottest hours (on halloweens). Implemented prime numbers.

11-02: Implemented way to rank days in terms of hotness

11-03: Added support for user selecting a date of interest

11-04: Added plotting of ranking

11-05: Improved efficiency of algorithms from sluggish to near-instant
