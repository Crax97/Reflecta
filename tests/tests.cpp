#include "doctest.h"

#include <memory>

#include "object.h"
#include "meta_descriptor.h"
#include "box_objects.h"

using namespace Reflecta;

class MyTestClass : public Object {
    
    OBJECT_BASE()

    public:
    int IntegerMember;
    float FloatMember;
    bool BooleanMember;

    int CoolFunc(int a, int b, int c) { 
        return a + b + c + IntegerMember;
    }
};

REFLECTA_BEGIN(MyTestClass)
    REFLECTA_REFLECT_MEMBER(IntegerMember)
    REFLECTA_REFLECT_MEMBER(FloatMember)
    REFLECTA_REFLECT_MEMBER(BooleanMember)
    REFLECTA_REFLECT_METHOD(&MyTestClass::CoolFunc)
REFLECTA_END(MyTestClass)

TEST_CASE("Ensure that REFLECTA_OFFSETOF works") {
    struct POD {
        int A;
        float B;
        char C;
        double D;
    };

    CHECK(REFLECTA_OFFSETOF(POD, A) == offsetof(POD, A));
    CHECK(REFLECTA_OFFSETOF(POD, B) == offsetof(POD, B));
    CHECK(REFLECTA_OFFSETOF(POD, C) == offsetof(POD, C));
    CHECK(REFLECTA_OFFSETOF(POD, D) == offsetof(POD, D));
}

TEST_CASE("Ensure that class descriptors exist") {
    auto class_instance = std::make_unique<MyTestClass>();
    auto descriptor = class_instance->get_meta_descriptor();
    CHECK(descriptor != nullptr);
}

TEST_CASE("Ensuring that properties are correctly reflected") {
    auto class_instance = std::make_unique<MyTestClass>();
    auto descriptor = class_instance->get_meta_descriptor();
    CHECK(descriptor->get_methods().size() == 1);
    CHECK(descriptor->get_instance_members().size() == 3);
    CHECK(descriptor->get_property("IntegerMember")->offset_in_class != 0);
    CHECK(descriptor->get_property("IntegerMember")->descriptor == Reflecta::get_meta_descriptor<int>());
}

TEST_CASE("Ensuring that property setter and getters work") {
    auto class_instance = std::make_unique<MyTestClass>();
    class_instance->set_property("IntegerMember", 128);
    class_instance->set_property("FloatMember", 42.0f);
    class_instance->set_property("BooleanMember", true);

    CHECK(class_instance->get_property<int>("IntegerMember") == 128);
    CHECK(class_instance->get_property<float>("FloatMember") == 42.0f);
    CHECK(class_instance->get_property<bool>("BooleanMember") == true);

    CHECK(class_instance->IntegerMember == 128);
    CHECK(class_instance->FloatMember == 42.0f);
    CHECK(class_instance->BooleanMember == true);
}

TEST_CASE("Ensure that non-existing properties result in nullopt") {
    auto class_instance = std::make_unique<MyTestClass>();
    CHECK(class_instance->get_property<int>("Nonexistent") == std::nullopt);
}

TEST_CASE("Ensure that values are correctly boxed") {
    int a = 10;
    long b = 23;
    float c = 23.24f;
    double d = 123.321;
    bool yes = true;

    CHECK(std::dynamic_pointer_cast<IntObject>(to_boxed_object(a))->value() == a);
    CHECK(std::dynamic_pointer_cast<LongObject>(to_boxed_object(b))->value() == b);
    CHECK(std::dynamic_pointer_cast<BoolObject>(to_boxed_object(yes))->value() == yes);
    CHECK(std::dynamic_pointer_cast<FloatObject>(to_boxed_object(c))->value() == c);
    CHECK(std::dynamic_pointer_cast<DoubleObject>(to_boxed_object(d))->value() == d);
}

TEST_CASE("Ensuring that calling reflected methods works") {
    auto class_instance = std::make_unique<MyTestClass>();
    class_instance->IntegerMember = 2;
    std::shared_ptr<MethodDescriptor> function = class_instance->get_method("CoolFunc").value();
    auto return_value = function->call<int>(class_instance.get()).value();
    CHECK(return_value->is(Reflecta::get_meta_descriptor<Reflecta::IntObject>()));

    auto integer_object = std::dynamic_pointer_cast<Reflecta::IntObject>(return_value);
    CHECK(integer_object->value() == 8);

}