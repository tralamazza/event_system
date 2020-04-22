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

## Build instructions

### Run demo

I kept my `main.cpp` playground where I tested some stuff during development.

In order to build and run this just do the following in the root folder of the project:
1. `make check` (build and run)
or separately via `make` and `./main`

### Run Tests

This solution is using the googletest framework for C++ unit tests.

1. Clone googletest into the root folder of this project via `git clone https://github.com/google/googletest.git`
2. Run `make check` inside the `tests` folder in order to compile and run the tests.

## Design choice, caveats and possible improvements

### Design choice

The event system consists out of a Dispatcher class and an abstract Event class.
An Observer can register a callback function for special Events at the Dispatcher.
A Client can trigger such an Event via the `Dispatcher::trigger()` method and the Event
will be forwarded to all registered Observers.

The Event class is an abstract base class. A custom Event has to extend this base class.
Different Events are distinguished by their name (e.g. BufferFullEvent). In comparison to
a simple integer id, a name does prevent duplicated custom Events a little bit better.
On the other hand it consumes more storage. At this point it depends - whether to use a simple
integer or a name (string) - on how many custom Events will be created. If thousands of different
Events are planned and available storage is low an simple integer might be better.
Besides that it is not guaranteed that two Events with the
same name are created at all. The Dispatcher can not distinguish them.

An Observer can be removed by the Dispatcher by calling `unregisterCallback()` providing the id
returned from `registerCallback()`.

### Caveats

In order to prevent adding/removing new Callbacks to the Dispatcher during an Event is
forwarded a mutex was introduced.

This mutex is used for registerCallback(), unregisterCallback() and trigger().

Not covered in this solution is the following scenario:

An Observer might register two Callbacks for the same Event. This can result in
race conditions on the Observer side. In this approach the Observer has to ensure
that it is able to handle his own callbacks in a way that those are not interfering each other.


### Possible improvements

- Error handling
    - Currently there are no checks that e.g. a `registerCallback()` or `unregisterCallback()`
    was successfully
- Extending Events
    - Currently an Event has only a name. There is no additional data/information.
    E.g. for a TouchEvent an Observer might want to know where the touch was recognized.
    This can be achieved by extending the custom TouchEvent. Every custom Event can have
    its own extensions.
- Clients/Observers can unregister callbacks from other Observers.
    - Since the registerCallback() returns a simple integer id this can be guessed by other
    Clients/Observers.
- Async Event notification
    - One Observer is able to block the Dispatcher via performing a lot of work in its
    Callback method.
