#pragma once

#include "meta_descriptor.h"

#include <string>
#include <sstream>
#include <string_view>
#include <memory>
#include <optional>

namespace Reflecta {

	class Object {

		public:
		virtual std::string to_string() const { 
			std::stringstream stream;
			stream << "Object @0x" << *reinterpret_cast<const long*>(this) << "\n";
			return stream.str();
		}

		virtual class MetaDescriptor* get_meta_descriptor();

		template<typename Type>
		std::optional<Type> set_property(const std::string_view& property_name, Type value) {
			auto property_descriptor = get_meta_descriptor()->get_property(property_name);
			if (property_descriptor == nullptr) {
				return std::nullopt;
			}
			auto* ptr = reinterpret_cast<Type*>(reinterpret_cast<char*>(this) + property_descriptor->offset_in_class);
			*ptr = value;
			
			return value;
		}

		template<typename Type>
		std::optional<Type> get_property(const std::string_view& property_name) {
			auto property_descriptor = get_meta_descriptor()->get_property(property_name);
			if (property_descriptor == nullptr) {
				return std::nullopt;
			}
			auto* ptr = reinterpret_cast<Type*>(reinterpret_cast<char*>(this) + property_descriptor->offset_in_class);
			return *ptr;
		}

		
		std::optional<std::shared_ptr<MethodDescriptor>> get_method(const std::string_view& property_name) {
			// TODO: implement
			return std::nullopt;
		}

		virtual bool is(class MetaDescriptor* descriptor) {
			return descriptor == get_meta_descriptor();
		}

	};
}

#define OBJECT_BASE() 										\
public:														\
virtual class MetaDescriptor* get_meta_descriptor();		\
private:	