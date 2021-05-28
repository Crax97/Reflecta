#pragma once

#include <string>
#include <sstream>
#include <string_view>

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
		Type set_property(const std::string_view& property_name, Type value);

		template<typename Type>
		Type get_property(const std::string_view& property_name);

	};

	template<>
	int Object::set_property<int>(const std::string_view& property_name, int value);
	template<>
	long Object::set_property<long>(const std::string_view& property_name, long value);
	template<>
	bool Object::set_property<bool>(const std::string_view& property_name, bool value);
	template<>
	float Object::set_property<float>(const std::string_view& property_name, float value);
	template<>
	double Object::set_property<double>(const std::string_view& property_name, double value);

	template<>
	int Object::get_property<int>(const std::string_view& property_name);

	template<>
	long Object::get_property<long>(const std::string_view& property_name);

	template<>
	bool Object::get_property<bool>(const std::string_view& property_name);

	template<>
	float Object::get_property<float>(const std::string_view& property_name);

	template<>
	double Object::get_property<double>(const std::string_view& property_name);

}

#define OBJECT_BASE() 										\
public:														\
virtual class MetaDescriptor* get_meta_descriptor();		\
private:	