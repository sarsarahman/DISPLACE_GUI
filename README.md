README
======

Summary
- Rationale
- How to cite
- How to install
- How to build
- GDAL notes



Rationale
============


DISPLACE: a dynamic, individual-based model for spatial fishing planning and effort displacement 
integrating underlying fish population models, check out the paper 2014 in 
Canadian Journal of Fisheries and Aquatic Sciences

The DISPLACE project develops and provides a platform primarily for research purposes 
to transform the fishermens detailed knowledge into models, evaluation tools, and methods 
that can provide the fisheries with research and advice. 
The model intends to serve as a basis for decision support tools for (fishery) managers.


How to cite
============

* Bastardie F, Nielsen JR, Miethe T. 2014. DISPLACE: a dynamic, individual-based model for 
spatial fishing planning and effort displacement - integrating underlying 
fish population models. Canadian Journal of Fisheries and Aquatic Sciences. 71(3):366-386. 
Available from: 10.1139/cjfas-2013-0126

* Bastardie, F., Nielsen, J. R., Eigaard, O. R., Fock, H. O., Jonsson, P., and Bartolino, V. 
Competition for marine space: modelling the Baltic Sea fisheries and effort displacement 
under spatial restrictions.  ICES Journal of Marine Science, doi: 10.1093/icesjms/fsu215.


How to install
============


Look at the Release section on the GitHub repository 
to download an installer for Windows. 

An example dataset is also available (DISPLACE_input)

or alternatively, look at the google drive for DISPLACE
also hosting additional files


How to compile
============
[compiling with CMake (preferred)](docs/Building-cmake.md)

[make the displace sdk (optional)](docs/Building.md)

[compiling on Windows (deprecated)](docs/Building.win)

[compiling on unix deprecated)](docs/Building.unix)

[compiling on MacOSX deprecated)](docs/Building.MacOSX)



GDAL Notes
==========

Current version of QMapControl supports shapefiles loading ONLY for WGS84 Coordinates system. This is because QMapControl itself uses WGS84 coordinates.
So you need to convert your shapefiles if not using this CRS.
You can use ogr2ogr or the provided script in the scripts/ directory.

$ ogr2ogr -t_srs WGS84 DEST.shp SRC.shp

Note that DEST file is specified before the source!


UNIT TESTING
============

Unit testing is performed using the Boost::Test framework. It can be compiled and linked in two ways:

- Dynamic linked, using the system installed boost::test library. This option is enabled by default on Unix
- Compiled in, using the boost/test/included/unit_test.hpp (included in main.cpp). This is suitable for Windows, where boost::test is not available with our version of mingw64. A standard installation of boost::test is required.

The two methods can be selected by defining boost_test_included in the CONFIG line of the Qt Project. It is enabled by default on Windows (see localconfig.pri).

If boost::test is not available in any form, it can be disabled by removing the units-test option from the CONFIG variable in the pro file.

