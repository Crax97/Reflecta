#include "object.h"

namespace Reflecta {
    class IntObject : public Object {
      OBJECT_BASE()
    public:
		std::string to_string() const override;
    int value() const;
    };
    class LongObject : public Object {
      OBJECT_BASE()
    public:
		std::string to_string() const override;
    long value() const;
    };
    class BoolObject : public Object {
      OBJECT_BASE()
    public:
		std::string to_string() const override;
    bool value() const;
    };
    class FloatObject : public Object {
      OBJECT_BASE()
    public:
		std::string to_string() const override;
    float value() const;
    };
    class DoubleObject : public Object {
      OBJECT_BASE()
    public:
		std::string to_string() const override;
    double value() const;
    };
}