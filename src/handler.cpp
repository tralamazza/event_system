#include "handler.h"

Handler::Handler()
{
    std::cout << "Creating handler for events" << std::endl;
}

Handler::~Handler()
{
    std::cout << "Destroying handler for events" << std::endl;
}

/**
 * Add pointer to the function to the end of array
 */
void Handler::add(std::function<void()> fcn)
{
    if(numCallbacks < MAX_CALLBACK_NUM)
    {
        handlerStorage.at(numCallbacks) = fcn;
        numCallbacks++;
    }
    else
    {
        std::cout << "Handler is full" << std::endl;
    }
    
}

/**
 * Trigger all registered functions
 */
void Handler::trigger()
{
    for(size_t i{0}; i < numCallbacks; i++)
    {
        handlerStorage.at(i)();
    }
}