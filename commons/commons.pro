TEMPLATE= lib
TARGET=displacecommons
DESTDIR=../
CONFIG += c++11 dll


include ("$$top_srcdir/localconfig.pri")
macx:DESTDIR=$$LIBDESTDIR

DEFINES += COMMONS_LIBRARY
INCLUDEPATH +=../include/ ../formats . ../sparsepp
LIBS += -L.. -lformats -lsqlite3

win32 {
    # No crash handler support for Windows
    DEFINES += NO_CRASHHANDLER

    # For GetProcessMemoryInfo()
    LIBS += -lpsapi
}

unix:!macx {
    LIBS += -lrt
}

macx {
    # No crash handler support for MacOS
    DEFINES += NO_CRASHHANDLER
}

message("Link Path: $$LIBS")

SOURCES= \
    readdata.cpp \
    myutils.cpp \
    Population.cpp \
    Fishfarm.cpp \
    Windmill.cpp \
    Node.cpp \
    Tac.cpp \
    Benthos.cpp \
    Harbour.cpp \
    Ship.cpp \
    Vessel.cpp \
    Metier.cpp \
    Firm.cpp \
    myRutils.cpp \
    mkpath.cpp \
    memoryinfo.cpp \
    options.cpp \
    statics.cpp \
    simulation.cpp \
    dtree/decisiontree.cpp \
    dtree/variables.cpp \
    dtree/dtnode.cpp \
    dtree/decisiontreemanager.cpp \
    dtree/externalstatemanager.cpp \
    tseries/timeseries.cpp \
    tseries/timeseriesmanager.cpp \
    utils/CrashHandler.cpp \
    pathshop.cpp \
    storage/sqlite/sqlitetable.cpp \
    storage/tables/vesseldeftable.cpp \
    storage/sqlite/sqlitestorage.cpp


HEADERS= \
    ../include/readdata.h \
    ../include/myutils.h \
    ../include/Population.h \
    ../include/Fishfarm.h \
    ../include/Windmill.h \
    ../include/Node.h \
    ../include/Tac.h \
    ../include/Benthos.h \
    ../include/Harbour.h \
    ../include/Ship.h \
    ../include/Vessel.h \
    ../include/Metier.h \
    ../include/Firm.h \
    ../include/myRutils.h \
    ../include/mkpath.h \
    ../include/m_constants.h \
    ../include/memoryinfo.h \
    ../include/helpers.h \
    ../include/options.h \
    ../include/guiproto_struct.h \
    ../include/version.h \
    ../include/utils/make_unique.h \
    ../include/vesselcalendar.h \
    ../include/idtypes.h \
    ../include/idtypeshelpers.h \
    simulation.h \
    dtree/decisiontree.h \
    dtree/variables.h \
    dtree/dtnode.h \
    dtree/dtnodeextra.h \
    dtree/decisiontreemanager.h \
    dtree/stateevaluator.h \
    dtree/externalstatemanager.h \
    dtree/commonstateevaluators.h \
    ../include/comstructs.h \
    tseries/timeseries.h \
    tseries/timeseriesmanager.h \
    dtree/evaluators/timeseriesevaluator.h \
    utils/CrashHandler.h \
    dtree/vesselsevaluators.h \
    commons_global.h \
    pathshop.h \
    storage/sqlite/sqlitetable.h \
    storage/sqlite/tables/vesseldeftable.h \
    storage/sqlite/sqlitestorage.h \
    storage/sqlite/sqlitefielddef.h


## Do not add this if you don't support IPC queues

!no_ipc {
    SOURCES += \
        ipcqueue.cpp

    HEADERS += \
        ../include/ipcqueue.h \
        ../include/ipcmsgtypes.h \
}

OTHER_FILES += \
    Makefile.hpc

target.path = $${PREFIX}/lib
headers.path = $${PREFIX}/include/
headers.files = $${HEADERS}

INSTALLS += target
