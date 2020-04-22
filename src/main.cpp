#include <iostream>
#include "handler.h"

/**
 * For method testing
 */
class TestClass
{
public:
    TestClass(){};
    ~TestClass(){};

    void method_callback() { std::cout << "This is method callback" << std::endl; }
};

/**
 * For standalone callback testing with different return type
 */
std::string NonMemberCallback()
{
    std::cout << "Hello from non member function!" << std::endl;
    return "Hello from non member function!";
}

/**
 * For standalone callback testing with no return value
 */
void test_callback()
{
    std::cout << "This is test callback!" << std::endl;
}

/**
 * For standalone callback with argument
 */
void testing(int a)
{
    std::cout << "A: " << a << std::endl;
}

/**
 * In main only tests for system
 */
int main() {
    std::cout << "Hello Easy C++ project!" << std::endl;

    Handler handler;
    TestClass testclass;

    /**
     * Test with lambda exprs
     */
    handler.add([]() { std::cout << "Event received in first callback!" << std::endl; });
    handler.add([]() { std::cout << "Event received in second callback!" << std::endl; });
    
    /**
     * Test with standalone callback
     */
    handler.add(&test_callback);
    handler.add([&testclass]() { testclass.method_callback(); });
    
    /**
     * Test using bind
     */
    auto callback1 = std::bind(&NonMemberCallback);
    handler.add(callback1);

    int a = 5;
    auto callback2 = std::bind(testing, a);
    handler.add(callback2);

    /**
     * Run all stored callbacks, in order in which they are added
     */   
    handler.trigger();

    return 0;
}