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

## Implementation

### Design and rationale

- The key for each `EventSystem` instance is a template,
  but expected to inherit from `Event`.
  The current implementation is incomplete and unsafe,
  as it doesn't really enforce a limit on the contained values.
  For production code this should be fixed,
  but for this demonstration it currently works well enough.
- The callbacks are stored in an array of vectors of callbacks,
  which makes accessing the callbacks performant with O(1).
  Also,
  it doesn't need a lock on *all* callbacks,
  as each event type has its own lock.

  Currently,
  `std::vector` is used for the sake of a simpler and more general implementation.
  Its major drawback is the allocation of dynamic memory.
  Alternatively,
  fixed-size arrays of callbacks could be used,
  but this also requires to remove the use of `std::function`,
  which *also* relies on dynamic allocations.

  The use of statically allocated memory would also be a better cache-coherency,
  which might impact performance positively.
- Callbacks are stored in a `std::function`, which can store methods, lambdas and
  standalone functions.
  The aforementioned concerns with dynamic allocations are also valid for this.

  An alternative would be the use of a type-erased container type with fixed-size storage.
  This would also make it possible to implement `operator==()` on the container type,
  which is necessary to make `std::remove()` work.
  Because `std::function` does not provide this operator,
  the unregistration of events is currently not implemented.
- The callback execution is controlled by the `Executor` template argument of the `Eventsystem` class.
  Two examples are provided:

  - A simple executor which just executes the callbacks on the thread that called `EventSystem::triggerEvent()`.
  - An implementation using a worker thread,
    which lets the caller continue after triggering the event.
    This could be extended to use a thread pool instead of a single worker thread if more performance was necessary.
    Another possibility would be a dedicated thread per event.

### Building and running

CMake (https://cmake.org) is used for building.
Tests can be executed by running `ctest`.

On Unix machines,
the following commands are sufficient:

```
$ mkdir build
$ cd build
$ cmake ..
...
$ cmake --build .
$ ctest -V
```
