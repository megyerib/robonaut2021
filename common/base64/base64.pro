TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += base64

SOURCES += \
	base64/base64.c \
	main.c

HEADERS += \
    base64/base64.h
