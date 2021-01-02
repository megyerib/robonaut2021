TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        MazeNavTest.cpp \
        MazeNav_AlwaysLeft.cpp \
        main.cpp

HEADERS += \
    MazeNavBase.h \
    MazeNavTest.h \
    MazeNav_AlwaysLeft.h \
    TestCases.h
