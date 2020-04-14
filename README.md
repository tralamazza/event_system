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

- The key for each `EventSystem` instance is generic,
  which makes it possible to use strong enumerations (i.e. `enum class`).

  The current approach is extremely flexible and allows the use of unbounded types,
  which makes it hard to prevent errors.
  The current C++ type system does not directly support the introspection of enum types to determine the number of elements,
  but a wrapper type which exposes the number could be used.
  But this makes it necessary to keep the event type enum and its wrapper type in sync,
  which is a huge drawback in itself.
  There are several ways around this limitations,
  but most involve the use of macros or elaborate template-meta-programming,
  which significantly hampers readability and make the implementation harder to understand.
  A third party solution is available on [GitHub](https://github.com/Neargye/magic_enum).
- Keys and callbacks are stored in a map of keys to vectors of callbacks,
  which makes accessing the callbacks reasonably performant with O(log(n)).

  If a type with a limited set of values,
  the storage could be a fixed-size array,
  which would reduce the access times to O(1).
  It also would reduce the necessary storage a bit,
  because the key must no longer be stored with the associated callbacks.
  Further,
  it could improve the performance by removing the mutex which protects the map from concurrent modifications,
  supposed it was contended.

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
- The Callbacks are executed on the thread which called `EventSystem::triggerEvent()`.
  If this thread was not allowed to block for the arbitrary time it takes the callback to execute,
  a worker thread or even a thread pool should be used to dispatch those callbacks.

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
