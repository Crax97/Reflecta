#pragma once

#include <string>
#include <sstream>

namespace Reflecta {
	class Object {

		virtual std::string to_string() const { 
			std::stringstream stream;
			stream << "Object @0x" << *reinterpret_cast<const long*>(this) << "\n";
			return stream.str();
		}

		virtual class MetaDescriptor* get_meta_descriptor();

	};
}