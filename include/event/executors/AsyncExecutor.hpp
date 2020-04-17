#ifndef EVENT_EXECUTORS_ASYNC_EXECUTOR_HPP_
#define EVENT_EXECUTORS_ASYNC_EXECUTOR_HPP_

#include <atomic>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>

#include <event/EventSystem.hpp>

namespace event {

template<typename Event>
class AsyncExecutor
{
public:
    using EventSystemType = EventSystem<Event, AsyncExecutor<Event>>;
    using CallbackStore = typename EventSystemType::CallbackStore;

    AsyncExecutor();
    ~AsyncExecutor();

    void execute(CallbackStore &callbacks, Event event)
    {
        std::lock_guard<std::mutex> lock { m_queueMtx };
        m_workQueue.emplace_back(&callbacks, event);
        m_workAvailableCondition.notify_one();
    }

private:
    struct WorkItem
    {
        // This is a pointer to make WorkItem default-constructible.
        CallbackStore *callbacks;
        Event event;
    };

    std::thread m_workerThread {};
    std::mutex m_queueMtx {};
    std::condition_variable m_workAvailableCondition;
    std::deque<Event> m_workQueue {};
    std::atomic_bool m_running { false };
};

template<typename Event>
AsyncExecutor<Event>::AsyncExecutor()
{
    m_running = true;
    m_workerThread = std::thread { [this]() {
        while (m_running) {
            WorkItem workItem {};
            {
                std::unique_lock<std::mutex> lock { m_queueMtx };
                if (m_workQueue.empty()) {
                    m_workAvailableCondition.wait(
                            lock, [this]() { return !m_workQueue.empty() || !m_running; });
                }
                if (!m_running) {
                    return;
                }
                workItem = m_workQueue.front();
                m_workQueue.pop_front();
            }
            // It is safe to always dereference the `callbacks` pointer here, because the `workItem`
            // instance is always copied into above.
            auto event = workItem.event;
            auto const &callbacks = workItem.callbacks->data;
            auto &mtx = workItem.callbacks.mtx;
            std::lock_guard<std::mutex> lock { workItem.callbacks->mtx };
            for (auto const &cb : callbacks) {
                cb(event);
            }
        }
    } };
}

template<typename Event>
AsyncExecutor<Event>::~AsyncExecutor()
{
    if (m_workerThread.joinable()) {
        m_running = false;
        m_workAvailableCondition.notify_one();
        m_workerThread.join();
    }
}

} // namespace event

#endif // EVENT_EXECUTORS_ASYNC_EXECUTOR_HPP_
