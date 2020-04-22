#ifndef HANDLER_H__
#define HANDLER_H__

#include <iostream>
#include <array>
#include <functional>

#define MAX_CALLBACK_NUM    256

/**
 * Handler class for handling callbacks, methods
 * - std::array used since this is memory efficient structure
 * - compile time is fixed, and compile size is fixed as well
 * - std::function used - lambda exprs, callbacks, methods, bind exprs
 * covered
 * - improvements:
 * -> add more types for callbacks(different arguments) - maybe use tuple for 
 * storing std::function
 * -> add remove functionality - limitation: comparing std::function for 
 * equality
 * -> if different storage types used (instead of std::array), change of 
 * Constructor/Destructor (different initialization for these structs)
 * -> adding more types for callbacks with different arguments will prevent 
 * usage binds in different part of program
 */
class Handler
{
    using array = std::array<std::function<void()>, MAX_CALLBACK_NUM>;
    
private:
    array handlerStorage;                               // storage for callbacks
    size_t numCallbacks{static_cast<size_t>(0)};        // number of callbacks currently
public:
    Handler();
    ~Handler();

    void add(std::function<void()>);
    void trigger();
};

#endif
