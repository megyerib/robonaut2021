TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    BinaryEncoder/EscapeEncoder.cpp \
    test.cpp

HEADERS += \
    BinaryEncoder/BinaryEncoder.h \
    BinaryEncoder/EscapeEncoder.h
