TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    MazeNavTest.cpp \
    MazeNav/MazeNav_AlwaysLeft.cpp \
    main.cpp

HEADERS += \
    MazeNav/MazeNavBase.h \
    MazeNavTest.h \
    MazeNav/MazeNav_AlwaysLeft.h \
    TestCases.h

INCLUDEPATH += \
    MazeNav/
