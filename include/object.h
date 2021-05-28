#pragma once

#include "meta_descriptor.h"

#include <string>
#include <sstream>
#include <string_view>
#include <memory>

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
		Type set_property(const std::string_view& property_name, Type value) {
			auto property_descriptor = get_meta_descriptor()->get_property(property_name);
			if (property_descriptor == nullptr) {
				// TODO Handle error!
			}
			auto* ptr = reinterpret_cast<Type*>(this + property_descriptor->offset_in_class);
			*ptr = value;
			
			return value;
		}

		template<typename Type>
		Type get_property(const std::string_view& property_name) {
			auto property_descriptor = get_meta_descriptor()->get_property(property_name);
			if (property_descriptor == nullptr) {
				// TODO Handle error!
			}
			auto* ptr = reinterpret_cast<Type*>(this + property_descriptor->offset_in_class);
			
			return *ptr;
		}

	};
}

#define OBJECT_BASE() 										\
public:														\
virtual class MetaDescriptor* get_meta_descriptor();		\
private:	