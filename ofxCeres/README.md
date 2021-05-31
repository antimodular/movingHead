# ofxCeres

OpenFrameworks addon / helpers for working with Google Ceres solver

http://ceres-solver.org/

# Installation

## OSX / XCode

After using the project generator you'll need to do one more step to make the the dylib from ceres ends up inside your app bundle.

* Add the libceres.2.0.0.dylib file into the Embedded Binaries section of Project Settings > General

You can see how to do that here:
[https://youtu.be/KnslJdFZp0w]

# Building Ceres notes

## OSX

* XCode project
* MinSizeRel
* Set the OSX deployment target to 10.9 (or whatever matches project generator this generation), can be done with CMAKE_OSX_DEPLOYMENT_TARGET
* Use the command to change the search path for the library : install_name_tool -id "@loader_path/../Frameworks/libceres.2.0.0.dylib" libceres.2.0.0.dylib 
