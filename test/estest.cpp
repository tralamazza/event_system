#include <event/EventSystem.hpp>
#include <event/executors/AsyncExecutor.hpp> // Just for compile tests
#include <event/executors/SimpleExecutor.hpp>

#include "catch.hpp"

namespace {

struct Event final : public event::Event<std::uint8_t, 3>
{
    constexpr Event(std::uint8_t i) : event::Event<std::uint8_t, 3> { i } {};
};

static constexpr Event StandaloneFunction { 0 };
static constexpr Event LambdaFunction { 1 };
static constexpr Event MemberFunction { 2 };

using EventSystem = event::EventSystem<Event, event::SimpleExecutor<Event>>;

bool g_standaloneFunctionWasCalled = false;

void standaloneFunction(Event /* ev */)
{
    g_standaloneFunctionWasCalled = true;
}

struct Callback
{
    bool wasCalled { false };

    void call(Event /* ev */) { wasCalled = true; }
};

} // namespace

SCENARIO("event system can register and remove callbacks")
{
    GIVEN("an event system")
    {
        EventSystem eventSystem {};

        WHEN("a standalone function is registered")
        {
            eventSystem.registerCallback(StandaloneFunction, standaloneFunction);

            THEN("it can be called")
            {
                eventSystem.triggerEvent(StandaloneFunction);

                REQUIRE(g_standaloneFunctionWasCalled == true);
            }
        }

        WHEN("a lambda is registered")
        {
            bool lambdaWasCalled = false;
            eventSystem.registerCallback(
                    LambdaFunction, [&lambdaWasCalled](Event /* ev */) { lambdaWasCalled = true; });

            THEN("it can be called")
            {
                eventSystem.triggerEvent(LambdaFunction);

                REQUIRE(lambdaWasCalled == true);
            }
        }

        WHEN("a member function is registered")
        {
            using std::placeholders::_1;

            Callback callback {};
            eventSystem.registerCallback(MemberFunction, std::bind(&Callback::call, &callback, _1));

            THEN("it can be called")
            {
                eventSystem.triggerEvent(MemberFunction);

                REQUIRE(callback.wasCalled == true);
            }
        }
    }
}
