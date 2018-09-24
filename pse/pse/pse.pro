TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    pscx_bios.cpp \
    pscx_cpu.cpp \
    pscx_instruction.cpp \
    pscx_interconnect.cpp \
    pscx_main.cpp

HEADERS += \
    pscx_bios.h \
    pscx_cpu.h \
    pscx_instruction.h \
    pscx_interconnect.h

