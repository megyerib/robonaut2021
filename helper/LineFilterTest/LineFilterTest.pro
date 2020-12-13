TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        ../../line-sensor/App/Line2020/src/LineFilter.c \
        main.cpp

HEADERS += \
    ../../common/Track/Line.h \
    ../../line-sensor/App/Line2020/inc/LineFilter.h

INCLUDEPATH += \
    ../../line-sensor/App/Line2020/inc \
    ../../common/Track
