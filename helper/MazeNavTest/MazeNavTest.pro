TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    MazeNav/dijkstra.cpp \
    MazeNav/mapnavigation.cpp \
    MazeNavTest.cpp \
    MazeNav/MazeNav_AlwaysLeft.cpp \
    main.cpp

HEADERS += \
    MazeNav/MazeNavBase.h \
    MazeNav/dijkstra.h \
    MazeNav/map_types.h \
    MazeNav/mapnavigation.h \
    MazeNav/static_map.h \
    MazeNavTest.h \
    MazeNav/MazeNav_AlwaysLeft.h \
    TestCases.h

INCLUDEPATH += \
    MazeNav/
