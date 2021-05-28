#include "meta_descriptor.h"

namespace Reflecta {
    template<>
	MetaDescriptor* get_descriptor<int>() {}

	template<>
	MetaDescriptor* get_descriptor<long>() {}

	template<>
	MetaDescriptor* get_descriptor<bool>() {}

	template<>
	MetaDescriptor* get_descriptor<float>() {}

	template<>
	MetaDescriptor* get_descriptor<double>() {}
}