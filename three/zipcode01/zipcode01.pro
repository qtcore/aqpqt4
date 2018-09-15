#-------------------------------------------------
#
# Project created by QtCreator 2018-09-14T21:35:35
#
#-------------------------------------------------

QT       += core  network


TARGET = zipcode01
CONFIG   += console debug
CONFIG   -= app_bundle


INCLUDEPATH  += ../aqp

SOURCES += main.cpp \
    uniqueproxymodel.cpp \
    standardtablemodel.cpp \
    proxymodel.cpp \
    mainwindow.cpp \
    main.cpp \
    itemdelegate.cpp \
    ../aqp/kuhn_munkres.cpp \
    ../aqp/aqp.cpp \
    ../aqp/alt_key.cpp

HEADERS += \
    zipcodespinbox.hpp \
    uniqueproxymodel.hpp \
    standardtablemodel.hpp \
    proxymodel.hpp \
    mainwindow.h \
    itemdelegate.hpp \
    global.hpp \
    ../aqp/kuhn_munkres.hpp \
    ../aqp/aqp.hpp \
    ../aqp/alt_key.hpp

RESOURCES += \
    ../aqp/aqp.qrc
debug {
    exists(../modeltest-0.2/modeltest.pri) {
        DEFINES += MODEL_TEST
        include(../modeltest-0.2/modeltest.pri)
    }
}
