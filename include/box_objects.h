#include "object.h"

namespace Reflecta {
    class IntObject : public Object {
		std::string to_string() const override;
		class MetaDescriptor* get_meta_descriptor() override;
    };
    class LongObject : public Object {
		std::string to_string() const override;
		class MetaDescriptor* get_meta_descriptor() override;
    };
    class BoolObject : public Object {
		std::string to_string() const override;
		class MetaDescriptor* get_meta_descriptor() override;
    };
    class FloatObject : public Object {
		std::string to_string() const override;
		class MetaDescriptor* get_meta_descriptor() override;
    };
    class DoubleObject : public Object {
		std::string to_string() const override;
		class MetaDescriptor* get_meta_descriptor() override;
    };
}