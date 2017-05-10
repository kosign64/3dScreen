TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    radio_lib.c \
    spi_lib.c

INCLUDEPATH += "/usr/lib/avr/include"

DEFINES += __AVR_ATmega128__

HEADERS += \
    nRF24L01.h \
    radio_lib.h \
    spi_lib.h
