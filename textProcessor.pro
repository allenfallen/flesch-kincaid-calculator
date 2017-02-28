TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    processor/processor.cpp

HEADERS += \
    processor/processor.h \
    containers/heap/functors.h \
    containers/heap/heap.h \
    containers/node/node.h \
    containers/pair/pair.h \
