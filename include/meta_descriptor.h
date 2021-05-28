#pragma once

#include <vector>
#include <unordered_map>
#include <initializer_list>
#include <memory>
#include <string_view>

#define REFLECTA_OFFSETOF(Type, Name) reinterpret_cast<size_t>(&static_cast<Type*>(nullptr)->Name)

namespace Reflecta {
	struct MethodDescriptor {
		const std::string_view MethodName;
		size_t offset_in_class;
		std::vector<class MetaDescriptor*> parameters;

		std::unique_ptr<class Object> call(std::initializer_list<std::unique_ptr<class Object>> values);
	};

	struct MemberDescriptor {
		size_t offset_in_class;
		class MetaDescriptor* descriptor;
		
		MemberDescriptor(size_t offset, class MetaDescriptor* member_descriptor) 
			: offset_in_class(offset), descriptor(member_descriptor) {}
	};

	class MetaDescriptor {
	private:
		std::string_view m_name;

	protected:
		std::vector<std::unique_ptr<MethodDescriptor>> instance_methods;
		std::unordered_map<std::string_view, std::shared_ptr<MemberDescriptor>> instance_members;

	public:

		MetaDescriptor(const std::string_view& name) : m_name(name) { }

		const std::string_view& get_name() const { return m_name; }
		const std::vector<std::unique_ptr<MethodDescriptor>>& get_methods() const {
			return instance_methods;
		}
		const std::unordered_map<std::string_view, std::shared_ptr<MemberDescriptor>>& get_instance_members() const {
			return instance_members;
		}

		const std::shared_ptr<MemberDescriptor> get_property(const std::string_view& property_name) const {

			const auto it = instance_members.find(property_name);
			if (it == instance_members.cend()) {
				return nullptr;
			}
			return it->second;
		}

		const virtual std::string_view& get_type_name() const { return m_name; };
	};

	template<typename Type>
	MetaDescriptor* get_meta_descriptor();

	template<>
	MetaDescriptor* get_meta_descriptor<bool>();

	template<>
	MetaDescriptor* get_meta_descriptor<int>();

	template<>
	MetaDescriptor* get_meta_descriptor<long>();

	template<>
	MetaDescriptor* get_meta_descriptor<float>();

	template<>
	MetaDescriptor* get_meta_descriptor<double>();

}												\

#define REFLECTA_BEGIN(Name) 							\
class Name ## _Descriptor : public MetaDescriptor { 	\
	using MyType = Name;								\
	public: 											\
	Name ## _Descriptor() : MetaDescriptor(#Name) { 	\


#define REFLECTA_REFLECT_MEMBER(MemberName) 								\
	instance_members.emplace(std::make_pair(#MemberName, 									\
		std::make_unique<MemberDescriptor>(REFLECTA_OFFSETOF(MyType, MemberName), 	\
		Reflecta::get_meta_descriptor<decltype(MyType::MemberName)>()))); 			\


#define REFLECTA_END(Name) 													\
} 																			\
};																			\
																			\
template<> 																	\
MetaDescriptor* Reflecta::get_meta_descriptor<Name>() {						\
	static Name ## _Descriptor* descriptor = new Name ## _Descriptor(); 	\
	return descriptor;														\
}																			\
class MetaDescriptor* Name::get_meta_descriptor() { 						\
	return Reflecta::get_meta_descriptor<Name>();							\
}																							