#pragma once

#include <vector>
#include <unordered_map>
#include <initializer_list>
#include <memory>

namespace Reflecta {
	struct MethodDescriptor {
		const std::string_view MethodName;
		long offset_in_class;
		std::vector<class MetaDescriptor*> parameters;

		std::unique_ptr<class Object> call(std::initializer_list<std::unique_ptr<class Object>> values);
	};

	class MetaDescriptor {
	private:
		std::string_view m_name;
		std::vector<std::unique_ptr<MethodDescriptor>> instance_methods;
		std::unordered_map<std::string_view, std::shared_ptr<MetaDescriptor>> instance_members;

	public:

		MetaDescriptor(const std::string_view& name) : m_name(name) { }

		const std::string_view& get_name() const { return m_name; }
		const std::vector<std::unique_ptr<MethodDescriptor>>& get_methods() const {
			return instance_methods;
		}
		const std::unordered_map<std::string_view, std::shared_ptr<MetaDescriptor>>& get_instance_members() const {
			return instance_members;
		}

		const virtual std::string_view& get_type_name() const { return m_name; };
	};

	template<typename Type>
	MetaDescriptor* get_descriptor();

	template<>
	MetaDescriptor* get_descriptor<int>();

	template<>
	MetaDescriptor* get_descriptor<long>();

	template<>
	MetaDescriptor* get_descriptor<bool>();

	template<>
	MetaDescriptor* get_descriptor<float>();

	template<>
	MetaDescriptor* get_descriptor<double>();

}

#define REFLECTA_BEGIN(Name) 							\
class Name ## _Descriptor : public MetaDescriptor { 	\
	public: 											\
	Name ## _Descriptor() : MetaDescriptor(#Name) { 	\


#define REFLECTA_REFLECT_MEMBER(MemberType, MemberName) \
	instance_members.insert({#MemberName, Reflecta::get_descriptor<MemberType>()}); \


#define REFLECTA_END(Name) 						\
} 											\
};											\
											\
template<> 									\
MetaDescriptor* get_descriptor<Name>() {	\
	static Name ## _Descriptor* descriptor = new Name ## _Descriptor(); \
	return descriptor;						\
}											