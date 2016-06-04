# School Regression Analysis

## Description

This readme describes the Regression Analysis for School Administrators application developed for UCLA CS 130, Spring 2016 by:

- Siddhant Khemka 
- Kevin Sheu 
- Kiran Sivakumar 
- Josh Vaughen 
- Karthik Vemulapalli 
- Dillon Zhi 

The application is intended to allow the non-technical user to perform basic statistical analyses on student data and to display the results within a GUI in an easy-to-understand format. The application also allows end users to export their session as a PDF for later viewing and use.

The application is supported on OS X 10.11 and Windows 10. 

## Building

Building the application is a fairly straightforward process:

1. Clone the repository to your local machine. 
2. Open the SchoolRegressionAnalysis project in Qt Creator 5.6+
3. Select the Release target in Qt Creator ![alt text](https://github.com/dzhi/school-regression-analysis/blob/master/img/target.png "Target")
4. Build the application
5. Optional: Run the application

## Installation

#### Windows

1. Download/run the application binary (SchoolRegressionAnalysis.exe)
2. Double-click the app to run it

#### Mac
1. Download/run the applicaiton binary (SchoolRegressionAnalysis.dmg)
2. Double-click the .dmg to mount the .dmg
3. Double-click the SchoolRegressionAnalysis app


## Usage

- To import data, first select the CSV source file on your filesystem

![alt text](https://github.com/dzhi/school-regression-analysis/blob/master/img/5.png "Select data source")
![alt text](https://github.com/dzhi/school-regression-analysis/blob/master/img/3.png "Select data source file")

- Once your data is loaded, analysis can be performed by selecting any of the two columns in the dataset using the dropdown menus labeled X and Y

![alt text](https://github.com/dzhi/school-regression-analysis/blob/master/img/1.png "Select columns")

- Once two columns are selected for inspection, press the "Select Columns" button to perform analysis

![alt text](https://github.com/dzhi/school-regression-analysis/blob/master/img/4.png "Perform analysis")

- If you desire a PDF copy of the visualization, click on File -> Export to export a PDF of the visualization

![alt text](https://github.com/dzhi/school-regression-analysis/blob/master/img/2.png "Export PDF")
