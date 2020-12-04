TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../../common/BinaryEncoder/BinaryEncoder/EscapeEncoder.cpp \
    ../../common/CRC/crc8/crc8.cpp \
    main.cpp

HEADERS += \
    ../../common/BinaryEncoder/BinaryEncoder/BinaryEncoder.h \
    ../../common/BinaryEncode/BinaryEncoderr/EscapeEncoder.h \
    ../../common/CRC/crc8/crc8.h \
    ../../common/Comm/inc/SerialMessages.h

INCLUDEPATH += \
    ../../common/Comm/inc/ \
    ../../common/CRC/crc8/ \
    ../../common/BinaryEncoder/BinaryEncoder/
