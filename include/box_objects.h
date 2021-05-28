#include "object.h"

namespace Reflecta {
    class IntObject : public Object {
      OBJECT_BASE()
      int m_value;
    public:
    IntObject(int value) : m_value(value) {}
		std::string to_string() const override {
      return std::string("IntObject") + std::to_string(m_value);
    }
    int value() const {
      return m_value;
    }
    };
    class LongObject : public Object {
      OBJECT_BASE()
      long m_value;
    public:
    LongObject(long value) : m_value(value) {}
		std::string to_string() const override {
      return std::string("LongObject") + std::to_string(m_value);
    }
    long value() const {
      return m_value;
    }
    };
    class BoolObject : public Object {
      OBJECT_BASE()
      bool m_value;
    public:
    BoolObject(bool value) : m_value(value) {}
		std::string to_string() const override {
      return std::string("BoolObject") + std::to_string(m_value);
    }
    bool value() const {
      return m_value;
    }
    };
    class FloatObject : public Object {
      OBJECT_BASE()
      float m_value;
    public:
    FloatObject(float value) : m_value(value) {}
		std::string to_string() const override {
      return std::string("FloatObject") + std::to_string(m_value);
    }
    float value() const {
      return m_value;
    }
    };
    class DoubleObject : public Object {
      OBJECT_BASE()
      double m_value;
    public:
    DoubleObject(double value) : m_value(value) {}
		std::string to_string() const override {
      return std::string("DoubleObject") + std::to_string(m_value);
    }
    double value() const {
      return m_value;
    }
    };
}