TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

HEADERS += \
    C++14/cpp14.hpp \
    C++17/1.hpp \
    C++17/3.hpp \
    C++17/4.hpp \
    C++17/constexpr_if.hpp \
    C++17/struct_binding.hpp \
    C++17/template_features.hpp \
    Matrix.hpp \
    VariadicTemplates/1.hpp \
    VariadicTemplates/2.hpp \
    VariadicTemplates/3.hpp \
    VariadicTemplates/4.hpp \
    VariadicTemplates/5.hpp \
    VariadicTemplates/6.hpp \
    bindings_and_algorithms_and_containers.hpp \
    designPatterns/abstract_factory_template.hpp \
    designPatterns/adapter_template.hpp \
    designPatterns/bridge_template.hpp \
    designPatterns/composite_template.hpp \
    designPatterns/decorator_template.hpp \
    designPatterns/facade_template.hpp \
    designPatterns/factory_method_template.hpp \
    designPatterns/prototype_template.hpp \
    filesystem.hpp \
    matrix.hpp \
    mutex.hpp \
    queue.hpp \
    stack.hpp \
    streams.hpp \
    thread_pool.hpp \
    time.hpp \
    utility.hpp

QMAKE_CXXFLAGS += -std=c++0x -pthread
LIBS += -pthread

DISTFILES +=
