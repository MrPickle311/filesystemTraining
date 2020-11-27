TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

HEADERS += \
    Matrix.hpp \
    filesystem.hpp \
    matrix.hpp \
    mutex.hpp \
    queue.hpp \
    stack.hpp \
    streams.hpp \
    time.hpp

QMAKE_CXXFLAGS += -std=c++0x -pthread
LIBS += -pthread
