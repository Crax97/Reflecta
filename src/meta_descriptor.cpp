#include "meta_descriptor.h"

namespace Reflecta {

	#define SIMPLE_DESCRIPTOR_NAME(Name) Name ## _Descriptor
	#define SIMPLE_DESCRIPTOR_DEFINITION(Name) \
	class Name ## _Descriptor : public Reflecta::MetaDescriptor { 	\
	public: 														\
		Name ## _Descriptor() : Reflecta::MetaDescriptor(#Name) {} 	\
}																	\

	#define IMPLEMENT_SIMPLE_DESCRIPTOR(Name) { 				\
	SIMPLE_DESCRIPTOR_DEFINITION(Name); 						\
	static auto* descriptor = new SIMPLE_DESCRIPTOR_NAME(Name);	\
	return descriptor; 											\
	}															\

	template<>
	MetaDescriptor* get_meta_descriptor<bool>() IMPLEMENT_SIMPLE_DESCRIPTOR(bool)

	template<>
	MetaDescriptor* get_meta_descriptor<int>() IMPLEMENT_SIMPLE_DESCRIPTOR(int)

	template<>
	MetaDescriptor* get_meta_descriptor<long>() IMPLEMENT_SIMPLE_DESCRIPTOR(long)

	template<>
	MetaDescriptor* get_meta_descriptor<float>() IMPLEMENT_SIMPLE_DESCRIPTOR(float)

	template<>
	MetaDescriptor* get_meta_descriptor<double>() IMPLEMENT_SIMPLE_DESCRIPTOR(double)

}