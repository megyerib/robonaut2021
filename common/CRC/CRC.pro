TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += crc8/

SOURCES += \
        crc8/crc8.cpp \
        main.cpp

HEADERS += \
        crc8/crc8.h
