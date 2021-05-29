#pragma once

#include <vector>
#include <unordered_map>
#include <initializer_list>
#include <memory>
#include <string_view>
#include <optional>
#include <vector>
#include <functional>

#define REFLECTA_OFFSETOF(Type, Name) reinterpret_cast<size_t>(&static_cast<Type*>(nullptr)->Name)

namespace Reflecta {


	template<typename Type>
	class MetaDescriptor* get_meta_descriptor();

	template <typename PrimitiveType>
	std::shared_ptr<class Object> to_boxed_object(PrimitiveType value);

	template <typename PrimitiveType>
	std::optional<PrimitiveType> unbox_to(const std::shared_ptr<Object>& object);

	struct MethodDescriptor {
		public:
		template<typename ReturnType, typename... Args>
		std::optional<std::shared_ptr<class Object>> call(class Object* instance, Args... args) {
			
			std::vector<std::shared_ptr<Object>> arg_list;
			return call_impl(instance, {to_boxed_object(args)...});
		}

		virtual std::optional<std::shared_ptr<class Object>> call_impl(class Object* instance,
			std::vector<std::shared_ptr<class Object>> values) = 0;
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

		const std::shared_ptr<MethodDescriptor> get_method(const std::string_view& method_name) const {

			const auto it = instance_methods.find(method_name);
			if (it == instance_methods.cend()) {
				return nullptr;
			}
			return it->second;
		}

		const virtual std::string_view& get_type_name() const { return m_name; };
	};
	template<typename BaseClass, typename ReturnType, typename ...Args, unsigned long... Indices>
	ReturnType call_helper(	BaseClass* derived, 
							ReturnType (BaseClass::*method_ptr)(Args... args), 
							std::vector<std::shared_ptr<class Object>> values, 
							std::index_sequence<Indices...>) {
		return (derived->*method_ptr)(unbox_to<Args>(values[Indices]).value()...);
	}
	template<typename BaseClass, typename ReturnType, typename ...Args>
	std::shared_ptr<MethodDescriptor> get_method_descriptor(ReturnType (BaseClass::*method_ptr)(Args... args)) {
		struct SpecializedMethodDescriptor : public MethodDescriptor {
			typedef  ReturnType (BaseClass::*MemberFn)(Args... args);
			MemberFn m_fn;

			public:
			SpecializedMethodDescriptor(MemberFn init_m_fn) : m_fn(init_m_fn) {}
			virtual std::optional<std::shared_ptr<class Object>> call_impl(class Object* instance,
						std::vector<std::shared_ptr<class Object>> values) override {
							auto derived = reinterpret_cast<BaseClass*>(instance);
							if (derived == nullptr) 
								return std::nullopt;

							auto return_value = call_helper(derived, m_fn, values, std::index_sequence_for<Args...>{});
							return to_boxed_object<decltype(return_value)>(return_value);
						}
		};

		static std::shared_ptr<MethodDescriptor> descriptor = std::make_shared<SpecializedMethodDescriptor>(method_ptr);

		return descriptor;
	}
}												

template<typename Base, typename Return, typename... Args>
Return get_return_type(Return (Base::*ptr)(Args... args));


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
	instance_methods.emplace(std::make_pair(#MethodName, 	\
	Reflecta::get_method_descriptor<MyType, decltype(get_return_type<MyType>(&MyType::MethodName))>(&MyType::MethodName)));


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