#ifndef EVENT_EXECUTORS_SIMPLE_EXECUTOR_HPP_
#define EVENT_EXECUTORS_SIMPLE_EXECUTOR_HPP_

#include <event/EventSystem.hpp>

namespace event {

/*!
 * @brief Simple executor, executing all callbacks in the calling thread.
 */
template<typename Event>
class SimpleExecutor
{
public:
    using EventSystemType = EventSystem<Event, SimpleExecutor<Event>>;
    using CallbackStore = typename EventSystemType::CallbackStore;

    void execute(CallbackStore &callbacks, Event event)
    {
        std::lock_guard<std::mutex> lock { callbacks.mtx };
        for (auto const &cb : callbacks.data) {
            cb(event);
        }
    }
};

} // namespace event

#endif // EVENT_EXECUTORS_SIMPLE_EXECUTOR_HPP_
