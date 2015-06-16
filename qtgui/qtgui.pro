TEMPLATE=app
QT += core gui network sql xml printsupport concurrent
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = displacegui

INCLUDEPATH+=../include/ ../QMapControl/QMapControl/src/ ../commons

include (../QMapControl/QMapControl/QMapControl.pri)
include ("$$top_srcdir/localconfig.pri")

DESTDIR = ../
QMAPCONTROL_LIB=qmapcontrol

CONFIG(debug,release|debug) {
    DEFINES += DEBUG
    QMAPCONTROL_LIB=qmapcontrold
}
CONFIG(release,release|debug) {
    QMAPCONTROL_LIB=qmapcontrol

# For use with debugger in release mode
#   QMAKE_CXXFLAGS_RELEASE += -g -O2
#   QMAKE_LFLAGS_RELEASE=
}

macx {
    CONFIG -= app_bundle
    ICON = icons/displace.icns

}

win32 {
    RC_FILE = qtgui.rc
    QMAPCONTROL_LIB=$${QMAPCONTROL_LIB}1

    CGAL_LIBS= -lCGAL -lgmp -lboost_system-mgw49-mt-1_57
}

unix {
    CGAL_LIBS= -lCGAL -lgmp
}

DEFINES += HAVE_GEOGRAPHICLIB
LIBS += -lGeographic

QMAKE_CXXFLAGS += -frounding-math
DEFINES += PROFILE
LIBS+=-L.. -ldisplacecommons -L../QMapControl/QMapControl/src/QMapControl/lib -l$$QMAPCONTROL_LIB $$CGAL_LIBS

# Add GDAL include path.
INCLUDEPATH += $$QMC_GDAL_INC

# Add GDAL library path and library (windows).
win32:LIBS += -L$$QMC_GDAL_LIB -lgdal

# Add GDAL library path and library (unix).
unix:LIBS += -lgdal

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    wrapperwidget.cpp \
    objecttreemodel.cpp \
    displacemodel.cpp \
    simulator.cpp \
    objects/objecttreeentity.cpp \
    objects/layerentity.cpp \
    objects/harbourentity.cpp \
    scenario.cpp \
    scenariodialog.cpp \
    config.cpp \
    objects/nodeentity.cpp \
    mapobjects/harbourmapobject.cpp \
    mapobjects/mapobject.cpp \
    mapobjectscontroller.cpp \
    mapobjects/nodemapobject.cpp \
    objects/vesselentity.cpp \
    mapobjects/vesselmapobject.cpp \
    objects/benthosentity.cpp \
    modelobjects/benthos.cpp \
    outputfileparser.cpp \
    simulationsetupdialog.cpp \
    dbhelper.cpp \
    modelobjects/nodedata.cpp \
    modelobjects/vesseldata.cpp \
    mapobjects/nodegraphics.cpp \
    editpalettedialog.cpp \
    palettemanager.cpp \
    qcustomplot.cpp \
    statscontroller.cpp \
    modelobjects/populationdata.cpp \
    objects/populationentity.cpp \
    graphinteractioncontroller.cpp \
    modelobjects/nationdata.cpp \
    objects/nationentity.cpp \
    objects/szgroupentity.cpp \
    configdialog.cpp \
    mapobjects/nodedetailswidget.cpp \
    modelobjects/harbourdata.cpp \
    mapobjects/edgemapobject.cpp \
    mapobjects/edgelayer.cpp \
    creategraphdialog.cpp \
    graphbuilder.cpp \
    calendar.cpp \
    waitdialog.cpp \
    backgroundworker.cpp \
    inputfileparser.cpp \
    shortestpathbuilder.cpp \
    mousemode.cpp \
    mousemode/drawpenaltypolygon.cpp \
    pathpenaltydialog.cpp \
    inputfileexporter.cpp \
    linkharboursdialog.cpp \
    mousemode/movefilteringmousemodedecorator.cpp \
    mousemode/singleclickmousemode.cpp \
    mousemode/edgeaddmousemode.cpp \
    aboutdialog.cpp \
    createshortestpathdialog.cpp \
    simulatoripcmanager.cpp \
    workers/shortestpathbuilderworker.cpp \
    csveditor.cpp \
    utils/csvtablemodel.cpp \
    utils/csvimporter.cpp \
    utils/csvexporter.cpp \
    mergedatadialog.cpp \
    workers/datamerger.cpp \
    utils/imageformathelpers.cpp \
    objects/shapefilelayerentity.cpp \
    utils/fileformathelper.cpp \
    shapefileoperationdialog.cpp \
    utils/mrupathmanager.cpp \
    savegraphdialog.cpp \
    workers/populationdistributiondatamergerstrategy.cpp \
    mergepopulationdatadialog.cpp \
    mergepopulationplugincomponent.cpp \
    workers/mergerstrategy.cpp \
    workers/graphbuilderworker.cpp \
    algo/isolatedsubgraphchecker.cpp \
    R/rconsole.cpp \
    R/env.cpp

FORMS += \
    mainwindow.ui \
    scenariodialog.ui \
    simulationsetupdialog.ui \
    editpalettedialog.ui \
    configdialog.ui \
    mapobjects/nodedetailswidget.ui \
    creategraphdialog.ui \
    waitdialog.ui \
    pathpenaltydialog.ui \
    linkharboursdialog.ui \
    aboutdialog.ui \
    createshortestpathdialog.ui \
    csveditor.ui \
    mergedatadialog.ui \
    shapefileoperationdialog.ui \
    savegraphdialog.ui \
    mergepopulationplugincomponent.ui \
    R/rconsole.ui

HEADERS += \
    mainwindow.h \
    wrapperwidget.h \
    objecttreemodel.h \
    displacemodel.h \
    simulator.h \
    objects/objecttreeentity.h \
    objects/layerentity.h \
    objects/harbourentity.h \
    scenario.h \
    exceptions.h \
    scenariodialog.h \
    config.h \
    objects/nodeentity.h \
    mapobjects/harbourmapobject.h \
    mapobjects/mapobject.h \
    mapobjectscontroller.h \
    mapobjects/nodemapobject.h \
    objects/vesselentity.h \
    mapobjects/vesselmapobject.h \
    objects/benthosentity.h \
    modelobjects/benthos.h \
    outputfileparser.h \
    simulationsetupdialog.h \
    dbhelper.h \
    modelobjects/nodedata.h \
    modelobjects/vesseldata.h \
    mapobjects/nodegraphics.h \
    editpalettedialog.h \
    palettemanager.h \
    qcustomplot.h \
    statscontroller.h \
    modelobjects/populationdata.h \
    objects/populationentity.h \
    ../include/profiler.h \
    historicaldatacollector.h \
    graphinteractioncontroller.h \
    modelobjects/nationdata.h \
    objects/nationentity.h \
    objects/szgroupentity.h \
    configdialog.h \
    mapobjects/nodedetailswidget.h \
    modelobjects/harbourdata.h \
    mapobjects/edgemapobject.h \
    mapobjects/edgelayer.h \
    ../include/memoryinfo.h \
    creategraphdialog.h \
    graphbuilder.h \
    calendar.h \
    waitdialog.h \
    backgroundworker.h \
    inputfileparser.h \
    shortestpathbuilder.h \
    mousemode.h \
    mousemode/drawpenaltypolygon.h \
    editorlayerinterface.h \
    pathpenaltydialog.h \
    inputfileexporter.h \
    linkharboursdialog.h \
    mousemode/movefilteringmousemodedecorator.h \
    mousemode/singleclickmousemode.h \
    mousemode/edgeaddmousemode.h \
    ../include/version.h \
    aboutdialog.h \
    createshortestpathdialog.h \
    simulatoripcmanager.h \
    workers/shortestpathbuilderworker.h \
    csveditor.h \
    utils/csvtablemodel.h \
    utils/csvimporter.h \
    utils/csvexporter.h \
    mergedatadialog.h \
    workers/datamerger.h \
    utils/imageformathelpers.h \
    objects/shapefilelayerentity.h \
    utils/fileformathelper.h \
    shapefileoperationdialog.h \
    utils/mrupathmanager.h \
    savegraphdialog.h \
    workers/populationdistributiondatamergerstrategy.h \
    utils/displaceexception.h \
    mergepopulationdatadialog.h \
    mergepopulationplugincomponent.h \
    workers/mergerstrategy.h \
    workers/graphbuilderworker.h \
    algo/isolatedsubgraphchecker.h \
    mapobjects/mapobjectcontainer.h \
    R/rconsole.h \
    R/env.h

RESOURCES += \
    qtgui.qrc

OTHER_FILES += \
    qtgui.rc \
    ../docs/output_fileformats.txt \
    palettes/heat_colors_12.p2c \
    ../docs/database.txt \
    ../docs/dbstruct.sh \
    palettes/pop_colors.p2c \
    ../docs/input_fileformats.txt \
    ../Doxyfile

TRANSLATIONS += \
    translations/displace_it.ts \
    translations/displace_fr.ts \
    translations/displace_de.ts


target.path=$${PREFIX}/bin

