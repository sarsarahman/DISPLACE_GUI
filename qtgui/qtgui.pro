TEMPLATE=app
QT += core gui network sql xml printsupport
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = displacegui

INCLUDEPATH+=../include/ ../QMapControl/QMapControl/src/

DESTDIR = ../
QMAPCONTROL_LIB=qmapcontrol

CONFIG(debug,release|debug) {
    QMAPCONTROL_LIB=qmapcontrold
}
CONFIG(release,release|debug) {
    QMAPCONTROL_LIB=qmapcontrol

# For use with callgrind
#    QMAKE_CXXFLAGS += -g -O2
}

macx {
    CONFIG -= app_bundle
    ICON = icons/displace.icns
}

win32 {
    RC_FILE = qtgui.rc
    QMAPCONTROL_LIB=$${QMAPCONTROL_LIB}1
}

DEFINES += PROFILE
LIBS+=-L.. -ldisplacecommons -L../QMapControl/QMapControl/src/QMapControl/lib -l$$QMAPCONTROL_LIB

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
    objects/outputlayerentity.cpp \
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
    configdialog.cpp

FORMS += \
    mainwindow.ui \
    scenariodialog.ui \
    simulationsetupdialog.ui \
    editpalettedialog.ui \
    configdialog.ui

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
    objects/outputlayerentity.h \
    editpalettedialog.h \
    palettemanager.h \
    qcustomplot.h \
    statscontroller.h \
    modelobjects/populationdata.h \
    objects/populationentity.h \
    profiler.h \
    historicaldatacollector.h \
    graphinteractioncontroller.h \
    modelobjects/nationdata.h \
    objects/nationentity.h \
    objects/szgroupentity.h \
    configdialog.h

RESOURCES += \
    qtgui.qrc

OTHER_FILES += \
    qtgui.rc \
    ../docs/output_fileformats.txt \
    palettes/iso1996_2.p2c
