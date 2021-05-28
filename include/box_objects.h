#include "object.h"

namespace Reflecta {
    class IntObject : public Object {
    public:
		std::string to_string() const override;
		class MetaDescriptor* get_meta_descriptor() override;
    int value() const;
    };
    class LongObject : public Object {
    public:
		std::string to_string() const override;
		class MetaDescriptor* get_meta_descriptor() override;
    long value() const;
    };
    class BoolObject : public Object {
    public:
		std::string to_string() const override;
		class MetaDescriptor* get_meta_descriptor() override;
    bool value() const;
    };
    class FloatObject : public Object {
    public:
		std::string to_string() const override;
		class MetaDescriptor* get_meta_descriptor() override;
    float value() const;
    };
    class DoubleObject : public Object {
    public:
		std::string to_string() const override;
		class MetaDescriptor* get_meta_descriptor() override;
    double value() const;
    };
}