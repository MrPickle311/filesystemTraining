TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

HEADERS += \
    C++14/cpp14.hpp \
    C++17/1.hpp \
    C++17/constexpr_if.hpp \
    C++17/iterator_features.hpp \
    C++17/milascenious.hpp \
    C++17/struct_binding_template.hpp \
    C++17/template_features.hpp \
    Matrix.hpp \
    VariadicTemplates/1.hpp \
    VariadicTemplates/2.hpp \
    VariadicTemplates/3.hpp \
    VariadicTemplates/4.hpp \
    VariadicTemplates/5.hpp \
    VariadicTemplates/6.hpp \
    bindings_and_algorithms_and_containers.hpp \
    concurrency/interrupt_thread.hpp \
    concurrency/joining_thread.hpp \
    concurrency/mutex.hpp \
    concurrency/queue.hpp \
    concurrency/stack.hpp \
    concurrency/thread_pool.hpp \
    concurrency/time.hpp \
    designPatterns/abstract_factory_template.hpp \
    designPatterns/adapter_template.hpp \
    designPatterns/bridge_template.hpp \
    designPatterns/command_template.hpp \
    designPatterns/composite_template.hpp \
    designPatterns/decorator_template.hpp \
    designPatterns/facade_template.hpp \
    designPatterns/factory_method_template.hpp \
    designPatterns/flyweight_template.hpp \
    designPatterns/iterator_template.hpp \
    designPatterns/mediator_template.hpp \
    designPatterns/memento_template.hpp \
    designPatterns/observer_template.hpp \
    designPatterns/prototype_template.hpp \
    designPatterns/proxy_template.hpp \
    designPatterns/responsibility_chain_template.hpp \
    designPatterns/state_template.hpp \
    filesystem.hpp \
    langFeatures/RTTI_dynamic_cast.hpp \
    langFeatures/class_hierarchies_1.hpp \
    langFeatures/class_hierarchies_2.hpp \
    langFeatures/classes_features.hpp \
    langFeatures/exceptions.hpp \
    langFeatures/exceptions_vol2.hpp \
    langFeatures/multiinheritance.hpp \
    langFeatures/operators.hpp \
    langFeatures/preprocesor.hpp \
    langFeatures/resource_management.hpp \
    langFeatures/specialisations.hpp \
    langFeatures/template_hierarchies.hpp \
    langFeatures/template_hierarchies_vol2.hpp \
    matrix.hpp \
    mutex.hpp \
    queue.hpp \
    stack.hpp \
    stl/bindings_and_algorithms_and_containers.hpp \
    stl/exeptions.hpp \
    stl/filesystem.hpp \
    stl/streams.hpp \
    stl/utility.hpp \
    streams.hpp \
    thread_pool.hpp \
    time.hpp \
    utility.hpp

QMAKE_CXXFLAGS += -std=c++0x -pthread
LIBS += -pthread

DISTFILES += \
    compile_features.txt
