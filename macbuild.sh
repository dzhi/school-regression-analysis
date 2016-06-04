#!/bin/sh

# this script will call upon macdeployqt in order to build a deployable .dmg file on OS X
# this script is called from a custom build step that occurs after linking, as specified in the .pro file
# for the project.

echo "RUNNING MACDEPLOYQT"
rm SchoolRegressionAnalysis.dmg
macdeployqt SchoolRegressionAnalysis.app -dmg -verbose=3
echo "MACDEPLOYQT FINISHED"
