#!/bin/sh

# this script will call upon windeployqt in order to build a deployable .exe file on windows
# this script is called from a custom build step that occurs after linking, as specified in the .pro file
# for the project.

# this has not yet been tested as i do not have a windows machine


windeployqt.exe SchoolRegressionAnalysis.exe 
