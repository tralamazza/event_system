# event_system
C++ event system challenge

## requirements

- Written in C++
- Must compile (please include build instructions)
- Open a PR with your solution

## goal

Write an event system which allows the user to register callbacks (both methods and stand alone functions) and to trigger such events. Describe your solution, core design choice, caveats and possible improvements/directions.
Consider:
- Type safety (e.g. for keys)
- Memory requirements

## Solution

Template class to explicitly construct a function object and execute the function object with arguments using parameter pack. C++17 has invoke feature which executes any callable with any number of arguments using parameter pack.

EventSystem has interfaces which allows to register functions and Execute the function based on event id which is generated during registration or Execute all functions based on Event type.

## Core Design choice

1. Delegate function template with parameter pack template function
2. Event System class to register known interfaces
3. Function types can be auto generated using regular expression script, since function definitions are known at compile time
4. std::any to pass the arguments for registrations
5. Using std::ref to avoid copy of functions

## Caveats

1. Callable Registration and Execute interfaces needs to be created for each and every datatype with different qulaifiers
2. Number of arguments also increases the memory of the system
3. For passing datatype like std::string, delegate requires different function object using basic_string<char>

## Possible Improvements

1. Using std::vector<std::function<void, void (*)(void *)>> as common storage/generic callable container for all callables and give a generic event id for all events to invoke again (Tried this, faced issue in reinterpret_cast of callable).

