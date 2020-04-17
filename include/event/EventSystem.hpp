#ifndef EVENT_EVENT_SYSTEM_HPP_
#define EVENT_EVENT_SYSTEM_HPP_

#include <algorithm>
#include <array>
#include <functional>
#include <mutex>
#include <vector>

namespace event {

template<typename T, std::size_t Max>
class Event
{
public:
    static constexpr std::size_t Count = Max;

    constexpr Event(T i) noexcept : m_id { i }
    {
        // TODO: it should not possible to create instances with i >= Max
    }

    constexpr T id() const noexcept { return m_id; }

private:
    T m_id;
};

// TODO: with more template-meta-programming, the Key type could be restricted a bit more
// Concepts would really shine here.
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
    struct CallbackStore
    {
        std::vector<Callback> data;
        std::mutex mtx;
    };

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
    std::array<CallbackStore, Key::Count> m_storage {};
    Executor m_executor;
};

template<typename Key, typename Executor>
void EventSystem<Key, Executor>::registerCallback(Key k, Callback cb)
{
    auto &callbacks = m_storage[k.id()];
    std::lock_guard<std::mutex> lock { callbacks.mtx };
    callbacks.data.emplace_back(std::move(cb));
}

template<typename Key, typename Executor>
void EventSystem<Key, Executor>::triggerEvent(Key k)
{
    auto &callbacks = m_storage[k.id()];
    // Passing a reference here is safe, because the executor is a member of this instance, so the
    // reference does never outlive its storage.
    m_executor.execute(callbacks, k);
}

} // namespace event

#endif // EVENT_EVENT_SYSTEM_HPP_
