openbest
========

open source implementation of the Berkeley Earth Surface Temperature software

To build the code, use the following commands in the root directory of the package:

$> cmake .
$> make

Now, the libraries and binary applications are available in the 'lib' and 'bin' directories of the root directory.

Debug/Release builds
####################

To distinguish debug/release builds, use the CMAKE_BUILD_TYPE variable:

$> cmake -DCMAKE_BUILD_TYPE=debug .
$> make

Documentation
#############

To generate the Doxygen-styled documentation, use the target 'doc':

$> make doc

Installation
############

To install the directory to a specific path, use the CMAKE_ROOT to set the prefix of installation and use the 'make install' command to install. 
To install targets and headers, don't forget to have 'write' rights for the target directories (commonly: do this with root permissions). E.g.:

$> cmake -DCMAKE_INSTALL_PREFIX=/usr .
$> make
$> make install
