#include <event/EventSystem.hpp>
#include <event/executors/AsyncExecutor.hpp> // Just for compile tests
#include <event/executors/SimpleExecutor.hpp>

#include "catch.hpp"

namespace {

enum class Event {
    StandaloneFunction,
    LambdaFunction,
    MemberFunction,
};

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
            eventSystem.registerCallback(Event::StandaloneFunction, standaloneFunction);

            THEN("it can be called")
            {
                eventSystem.triggerEvent(Event::StandaloneFunction);

                REQUIRE(g_standaloneFunctionWasCalled == true);
            }
        }

        WHEN("a lambda is registered")
        {
            bool lambdaWasCalled = false;
            eventSystem.registerCallback(Event::LambdaFunction, [&lambdaWasCalled](Event /* ev */) {
                lambdaWasCalled = true;
            });

            THEN("it can be called")
            {
                eventSystem.triggerEvent(Event::LambdaFunction);

                REQUIRE(lambdaWasCalled == true);
            }
        }

        WHEN("a member function is registered")
        {
            using std::placeholders::_1;

            Callback callback {};
            eventSystem.registerCallback(Event::MemberFunction,
                                         std::bind(&Callback::call, &callback, _1));

            THEN("it can be called")
            {
                eventSystem.triggerEvent(Event::MemberFunction);

                REQUIRE(callback.wasCalled == true);
            }
        }
    }
}
