#include "meta_descriptor.h"

#include "box_objects.h"

#include <memory>

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

	template<>
	std::shared_ptr<class Object> to_boxed_object<int>(int value) {
		return std::make_shared<IntObject>(value);
	}
	template<>
	std::shared_ptr<class Object> to_boxed_object<long>(long value) {
		return std::make_shared<LongObject>(value);
	}
	template<>
	std::shared_ptr<class Object> to_boxed_object<bool>(bool value) {
		return std::make_shared<BoolObject>(value);
	}
	template<>
	std::shared_ptr<class Object> to_boxed_object<float>(float value) {
		return std::make_shared<FloatObject>(value);
	}
	template<>
	std::shared_ptr<class Object> to_boxed_object<double>(double value) {
		return std::make_shared<DoubleObject>(value);
	}

	template<>
	std::optional<int> unbox_to<int>(const std::shared_ptr<Object>& object) {
		if (object->get_meta_descriptor() == get_meta_descriptor<IntObject>()) {
			auto int_object = std::dynamic_pointer_cast<IntObject>(object);
			return int_object->value();
		}
		return std::nullopt;
	}

}