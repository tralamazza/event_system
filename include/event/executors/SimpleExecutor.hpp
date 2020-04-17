#ifndef EVENT_EXECUTORS_SIMPLE_EXECUTOR_HPP_
#define EVENT_EXECUTORS_SIMPLE_EXECUTOR_HPP_

namespace event {

/*!
 * @brief Simple executor, executing all callbacks in the calling thread.
 */
template<typename Event>
class SimpleExecutor
{
public:
    template<typename CallbackIterator>
    void execute(CallbackIterator first, CallbackIterator last, Event event)
    {
        for (auto iter = first; iter < last; ++iter) {
            (*iter)(event);
        }
    }
};

} // namespace event

#endif // EVENT_EXECUTORS_SIMPLE_EXECUTOR_HPP_
