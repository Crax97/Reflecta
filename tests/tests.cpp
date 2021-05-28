#include "doctest.h"

#include "object.h"
#include "meta_descriptor.h"

using namespace Reflecta;

class MyTestClass : public Object {
    
    OBJECT_BASE()

    public:
    int IntegerMember;
    float FloatMember;
    bool BooleanMember;

    int CoolFunc(int a, int b, int c) { 
        return a + b + c;
    }
};

REFLECTA_BEGIN(MyTestClass)
    REFLECTA_REFLECT_MEMBER(IntegerMember)
    REFLECTA_REFLECT_MEMBER(FloatMember)
    REFLECTA_REFLECT_MEMBER(BooleanMember)
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
    CHECK(descriptor->get_methods().size() == 0);
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
}