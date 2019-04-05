Find your way with DISPLACE
======

## Summary
- [What for?](#what-for)
- [How to cite](#how-to-cite)
- [How to install and run](#how-to-install)
- [How to compile from scratch](#how-to-compile)
- [GDAL notes](#gdal-notes)
- [Unit testing](#unit-testing)



## What for?


DISPLACE: a dynamic, individual-based model for spatial fishing planning and effort displacement 
integrating underlying fish population models. The DISPLACE project develops and provides a 
platform primarily for research purposes to transform the fishermens detailed knowledge into models,
 evaluation tools, and methods that can provide the fisheries with research and advice. 
The model intends to serve as a basis for decision support tools for (fishery) managers.


## How to cite

* Bastardie F, Nielsen JR, Miethe T. 2014. DISPLACE: a dynamic, individual-based model for 
spatial fishing planning and effort displacement - integrating underlying 
fish population models. Canadian Journal of Fisheries and Aquatic Sciences. 71(3):366-386. [link here](https://www.nrcresearchpress.com/doi/full/10.1139/cjfas-2013-0126#.XJs-ubh7nmE) 

* Bastardie, F., Nielsen, J. R., Eigaard, O. R., Fock, H. O., Jonsson, P., and Bartolino, V. 
Competition for marine space: modelling the Baltic Sea fisheries and effort displacement 
under spatial restrictions. ICES Journal of Marine Science, doi: 10.1093/icesjms/fsu215. [link to free copy](https://academic.oup.com/icesjms/article/72/3/824/701817) 

* Bastardie, F., Nielsen, J. R., Eero, M., Fuga, F. Rindorf., A. 2017. Effects of changes
in stock productivity and mixing on sustainable fishing and economic viability,
ICES Journal of Marine Science, Volume 74, Issue 2, Pages 535–551
[link to free copy](https://academic.oup.com/icesjms/article/74/2/535/2669542)

* Bastardie, F., Angelini, S., Bolognini, L., Fuga, F., Manfredi, C., Martinelli, M.,
Nielsen, J. R., Santojanni, A., Scarcella, G., and Grati, F.. 2017. 
Spatial planning for fisheries in the Northern Adriatic: working toward viable and sustainable fishing.
Ecosphere 8( 2):e01696. [link to free copy](https://esajournals.onlinelibrary.wiley.com/doi/full/10.1002/ecs2.1696) 



## Requirements

### MacOS

In MacOS, [brew](https://brew.sh) is required to install and run the Gui.

Refer to the Brew documentation on how to install it.

The following packages must be installed in brew:

- gdal


## How to install DISPLACE and run a simulation


Look at the [Release section](https://github.com/frabas/DISPLACE_GUI/releases) on this GitHub repository 
to download an installer for Windows or packages for Unix and MacOSX. or alternatively, look at the [google drive for DISPLACE](https://drive.google.com/drive/folders/0ByuO_4j-1PxtfnZBblpQNmh2a2Z4SmpkRC16T1kwR0t1RWUyOVUxdHlEZzZwZWVpaVJac00)
also hosting additional files

[DISPLACE Example datasets](https://displace-project.org/blog/download/) are available (DISPLACE_input_xx)

Run the graphical user interface for DISPLACE with e.g. displacegui

Quick start fo running a DISPLACE simulation: Make sure your model objects is set to [0]. If yes then in the main menu do a "File">"Load a Scenario Model" and choose a scenario file (a .dat file) you´ll find in the \simusspe folder of your DISPLACE dataset.


## How to compile from the code source


[compiling with CMake (preferred)](docs/Building-cmake.md)

[make the displace sdk (optional)](docs/building.md)

[compiling on Windows (deprecated)](docs/Building.win)

[compiling on unix deprecated)](docs/Building.unix)

[compiling on MacOSX deprecated)](docs/Building.MacOSX)


## GDAL Notes


Current version of QMapControl supports shapefiles loading ONLY for WGS84 Coordinates system. This is because QMapControl itself uses WGS84 coordinates.
So you need to convert your shapefiles if not using this CRS.
You can use ogr2ogr or the provided script in the scripts/ directory.

$ ogr2ogr -t_srs WGS84 DEST.shp SRC.shp

Note that DEST file is specified before the source!


## Unit testing


Unit testing is performed using the Boost::Test framework. It can be compiled and linked in two ways:

- Dynamic linked, using the system installed boost::test library. This option is enabled by default on Unix
- Compiled in, using the boost/test/included/unit_test.hpp (included in main.cpp). This is suitable for Windows, where boost::test is not available with our version of mingw64. A standard installation of boost::test is required.

The two methods can be selected by defining boost_test_included in the CONFIG line of the Qt Project. It is enabled by default on Windows (see localconfig.pri).

If boost::test is not available in any form, it can be disabled by removing the units-test option from the CONFIG variable in the pro file.
