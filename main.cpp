//#include "filesystem.hpp"
//#include "streams.hpp"
//#include "mutex.hpp"
//#include "time.hpp"
//#include "thread_pool.hpp"
#include <list>

#include "designPatterns/factory_method_template.hpp"
#include "designPatterns/abstract_factory_template.hpp"
#include "designPatterns/prototype_template.hpp"
#include "designPatterns/adapter_template.hpp"
#include "designPatterns/bridge_template.hpp"
#include "designPatterns/composite_template.hpp"
#include "designPatterns/facade_template.hpp"
#include "designPatterns/flyweight_template.hpp"
#include "designPatterns/proxy_template.hpp"
#include "designPatterns/responsibility_chain_template.hpp"

#include "C++17/struct_binding_template.hpp"
#include "C++14/cpp14.hpp"

#include "langFeatures/multiinheritance.hpp"
#include "langFeatures/RTTI_dynamic_cast.hpp"
#include "langFeatures/operators.hpp"

#include "stl/utility.hpp"

class x
{
public:
	int a;
};

class z:
		public x
{
public:
	int b;
};

int main(int argc,char* argv[])
{
	//x& zx {*new z};
	//niby fajnie ,że tak można,ale istnieje możliwy wyciek pamięci

	//operator_test();
	return 0;
}
