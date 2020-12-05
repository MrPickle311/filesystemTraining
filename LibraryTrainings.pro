TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

HEADERS += \
    Matrix.hpp \
    bindings_and_algorithms_and_containers.hpp \
    designPatterns/abstract_factory_template.hpp \
    designPatterns/factory_method_template.hpp \
    filesystem.hpp \
    matrix.hpp \
    mutex.hpp \
    queue.hpp \
    stack.hpp \
    streams.hpp \
    thread_pool.hpp \
    time.hpp

QMAKE_CXXFLAGS += -std=c++0x -pthread
LIBS += -pthread

DISTFILES +=
