#ifndef EVENT_EVENT_SYSTEM_HPP_
#define EVENT_EVENT_SYSTEM_HPP_

#include <algorithm>
#include <functional>
#include <map>
#include <mutex>
#include <vector>

namespace event {

/*!
 * @brief A generic event system.
 *
 * Events can be registered and triggered.
 */
template<typename Key, typename Executor>
class EventSystem
{
public:
    using Callback = std::function<void(Key)>;

    /*!
     * @brief Creates a new instance of the event system with a default instance of the executor.
     */
    EventSystem() : m_executor {} {};

    /*!
     * @brief Creates a new instance of the event system with the executor instance.
     *
     * @param executor The executor instance to use.
     */
    EventSystem(Executor executor) : m_executor { std::move(executor) } { }

    /*!
     * @brief Registers a callback for an event.
     *
     * @param k Event for which the callback ie registered.
     * @param cb Callback which will be executed when the event is triggered.
     */
    void registerCallback(Key k, Callback cb);

    /*!
     * @brief Triggers an event.
     *
     * @param k The event to execute the callbacks for.
     */
    void triggerEvent(Key k);

private:
    std::mutex m_mapAccess {};
    std::map<Key, std::vector<Callback>> m_callbacks {};
    Executor m_executor;
};

template<typename Key, typename Executor>
void EventSystem<Key, Executor>::registerCallback(Key k, Callback cb)
{
    std::lock_guard<std::mutex> lock { m_mapAccess };
    auto &callbacks = m_callbacks[k];
    callbacks.emplace_back(std::move(cb));
}

template<typename Key, typename Executor>
void EventSystem<Key, Executor>::triggerEvent(Key k)
{
    std::lock_guard<std::mutex> lock { m_mapAccess };
    auto const &callbacks = m_callbacks[k];
    m_executor.execute(std::begin(callbacks), std::end(callbacks), k);
}

} // namespace event

#endif // EVENT_EVENT_SYSTEM_HPP_
