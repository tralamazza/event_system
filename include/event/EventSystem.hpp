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
template<typename Key>
class EventSystem
{
public:
    using Callback = std::function<void(Key)>;

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
    std::mutex m_mapAccess{};
    std::map<Key, std::vector<Callback>> m_callbacks{};
};

template<typename Key>
void EventSystem<Key>::registerCallback(Key k, Callback cb)
{
    std::lock_guard<std::mutex> lock{m_mapAccess};
    auto &callbacks = m_callbacks[k];
    callbacks.emplace_back(std::move(cb));
}

template<typename Key>
void EventSystem<Key>::triggerEvent(Key k)
{
    std::lock_guard<std::mutex> lock{m_mapAccess};
    auto const &callbacks = m_callbacks[k];
    for (auto const &cb : callbacks) {
        cb(k);
    }
}

} // namespace event

#endif // EVENT_EVENT_SYSTEM_HPP_
