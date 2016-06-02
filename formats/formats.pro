CONFIG -= qt

TEMPLATE= lib
TARGET=formats
DESTDIR=../
CONFIG += c++11

include ("$$top_srcdir/localconfig.pri")
macx:DESTDIR=$$LIBDESTDIR

INCLUDEPATH += .

win32 {
}

unix:!macx {
}

HEADERS = \
    legacy/ConfigFileReader.h \
    utils/KeyValueReader.h \
    utils/LineNumberReader.h \
    utils/prettyprint.h \
    legacy/NodesFileReader.h utils/MultifieldReader.h \
    utils/vectorsdata.h \
    formatexception.h \
    utils/MultifieldWriter.h \
    legacy/binarygraphfilereader.h \
    utils/endian.h \
    legacy/binarygraphfilewriter.h

SOURCES = legacy/ConfigFileReader.cpp  \
    utils/KeyValueReader.cpp \
    utils/LineNumberReader.cpp

OTHER_FILES = Makefile.hpc

win32 {
    LIBS +=
}

!win32 {
    LIBS +=
}

target.path = $${PREFIX}/lib
headers.path = $${PREFIX}/include/
headers.files = $${HEADERS}

INSTALLS += target

