#pragma once

#include <vector>
#include <unordered_map>
#include <initializer_list>
#include <memory>
#include <string_view>
#include <optional>

#define REFLECTA_OFFSETOF(Type, Name) reinterpret_cast<size_t>(&static_cast<Type*>(nullptr)->Name)

namespace Reflecta {


	template<typename Type>
	class MetaDescriptor* get_meta_descriptor();

	template<>
	class MetaDescriptor* get_meta_descriptor<bool>();

	template<>
	class MetaDescriptor* get_meta_descriptor<int>();

	template<>
	class MetaDescriptor* get_meta_descriptor<long>();

	template<>
	class MetaDescriptor* get_meta_descriptor<float>();

	template<>
	class MetaDescriptor* get_meta_descriptor<double>();


	template <typename PrimitiveType>
	std::unique_ptr<class Object> to_boxed_object(PrimitiveType value);

	struct MethodDescriptor {
		public:
		template<typename ReturnType, typename... Args>
		std::optional<std::shared_ptr<class Object>> call(class Object* instance, Args... args) {
			// TODO: Implement
			return call_impl(instance, {to_boxed_object<Args>(std::forward<Args>(args)) ...});
		}

		virtual std::optional<std::shared_ptr<class Object>> call_impl(class Object* instance,
			std::initializer_list<std::unique_ptr<class Object>> values) = 0;
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
		std::unordered_map<std::string_view, std::shared_ptr<MethodDescriptor>> instance_methods;
		std::unordered_map<std::string_view, std::shared_ptr<MemberDescriptor>> instance_members;

	public:

		MetaDescriptor(const std::string_view& name) : m_name(name) { }

		const std::string_view& get_name() const { return m_name; }
		const std::unordered_map<std::string_view, std::shared_ptr<MethodDescriptor>>& get_methods() const {
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
	template<typename FPtrType>
	std::shared_ptr<MethodDescriptor> get_method_descriptor() {
		return nullptr;
	}
}												\

#define REFLECTA_BEGIN(Name) 							\
class Name ## _Descriptor : public MetaDescriptor { 	\
	using MyType = Name;								\
	public: 											\
	Name ## _Descriptor() : MetaDescriptor(#Name) { 	\


#define REFLECTA_REFLECT_MEMBER(MemberName) 								\
	instance_members.emplace(std::make_pair(#MemberName, 									\
		std::make_unique<MemberDescriptor>(REFLECTA_OFFSETOF(MyType, MemberName), 	\
		Reflecta::get_meta_descriptor<decltype(MyType::MemberName)>()))); 	


#define REFLECTA_REFLECT_METHOD(MethodName) 								\
	instance_methods.emplace(std::make_pair(#MethodName, Reflecta::get_method_descriptor<decltype(MethodName)>()));


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