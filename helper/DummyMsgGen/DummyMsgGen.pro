TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../../common/BinaryEncoder/EscapeEncoder.cpp \
    ../../common/CRC/crc8.cpp \
    main.cpp

HEADERS += \
    ../../common/BinaryEncoder/BinaryEncoder.h \
    ../../common/BinaryEncoder/EscapeEncoder.h \
    ../../common/CRC/crc8.h \
    ../../common/SerialMessages/SerialMessages.h

INCLUDEPATH += \
    ../../common/SerialMessages/ \
    ../../common/CRC/ \
    ../../common/BinaryEncoder/
