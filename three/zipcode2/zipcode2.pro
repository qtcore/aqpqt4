CONFIG	     += console debug
QT           += network
HEADERS	     += ../aqp/kuhn_munkres.hpp \
    ../zipcode01/mainwindow.h
SOURCES	     += ../aqp/kuhn_munkres.cpp
HEADERS	     += ../aqp/alt_key.hpp
SOURCES	     += ../aqp/alt_key.cpp
HEADERS	     += ../aqp/aqp.hpp
SOURCES	     += ../aqp/aqp.cpp
RESOURCES    += ../aqp/aqp.qrc
INCLUDEPATH  += ../aqp
HEADERS	     += ../zipcode01/global.hpp
HEADERS	     += ../zipcode01/zipcodespinbox.hpp
HEADERS	     += ../zipcode01/itemdelegate.hpp
SOURCES	     += ../zipcode01/itemdelegate.cpp
HEADERS	     += ../zipcode01/proxymodel.hpp
SOURCES	     += ../zipcode01/proxymodel.cpp
HEADERS	     += ../zipcode01/uniqueproxymodel.hpp
SOURCES	     += ../zipcode01/uniqueproxymodel.cpp
HEADERS	     += ../zipcode01/mainwindow.hpp
SOURCES      += ../zipcode01/mainwindow.cpp
SOURCES      += ../zipcode01/main.cpp
TRANSLATIONS += ../zipcode01/zipcodes_en.ts
INCLUDEPATH  += ../zipcode01
DEFINES	     += CUSTOM_MODEL
HEADERS	     += zipcodeitem.hpp
HEADERS	     += tablemodel.hpp
SOURCES      += tablemodel.cpp
win32 { INCLUDEPATH += . }

debug {
exists(../modeltest-0.2/modeltest.pri) {
    DEFINES += MODEL_TEST
    include(../modeltest-0.2/modeltest.pri)
}
}
