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

## Event system based on Observer design pattern

Event system based on observer pattern in which object (subject), creates list of dependents (interfaces) and notifies on any changes on particular method. Short example created to demonstrate concept.

### How to build

Dependencies:
	- Cmake
	- Ninja/make
	- gcc/g++ compiler

1) Clone this repository `cd <dir>`
2) Navigate to build directory `cd build`
3) Execute `cmake .. -G Ninja` or `cmake .. -G make`
4) In build directory run `ninja` or `make`
5) Run example with `./callbak-engine`

### Different approach and why it was avoided

Callback function could be also defined using lambda expressions and with std::function trough bind method for class static/non-static methods and non class methods. With observer pattern whole objects are registered with all members and methods which can perform complicated operations when event occurs. With lambda functions it would be difficult to register capture for more complex functions. With observer pattern we are storing vector of object pointers and not only wrappers for function pointers.


