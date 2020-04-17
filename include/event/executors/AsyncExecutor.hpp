#ifndef EVENT_EXECUTORS_ASYNC_EXECUTOR_HPP_
#define EVENT_EXECUTORS_ASYNC_EXECUTOR_HPP_

#include <atomic>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>

namespace event {

template<typename Event, typename CallbackIterator>
class AsyncExecutor
{
public:
    AsyncExecutor();
    ~AsyncExecutor();

    void execute(CallbackIterator first, CallbackIterator last, Event event)
    {
        std::lock_guard<std::mutex> lock { m_queueMtx };
        m_workQueue.emplace_back(first, last, event);
        m_workAvailableCondition.notify_one();
    }

private:
    struct WorkItem
    {
        CallbackIterator first;
        CallbackIterator last;
        Event event;
    };

    std::thread m_workerThread {};
    std::mutex m_queueMtx {};
    std::condition_variable m_workAvailableCondition;
    std::deque<Event> m_workQueue {};
    std::atomic_bool m_running { false };
};

template<typename Event, typename CallbackIterator>
AsyncExecutor<Event, CallbackIterator>::AsyncExecutor()
{
    m_running = true;
    m_workerThread = std::thread { [this]() {
        WorkItem workItem {};
        while (m_running) {
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
            for (auto iter = workItem.first; iter < workItem.last; ++iter) {
                (*iter)(workItem.event);
            }
        }
    } };
}

template<typename Event, typename CallbackIterator>
AsyncExecutor<Event, CallbackIterator>::~AsyncExecutor()
{
    if (m_workerThread.joinable()) {
        m_running = false;
        m_workAvailableCondition.notify_one();
        m_workerThread.join();
    }
}

} // namespace event

#endif // EVENT_EXECUTORS_ASYNC_EXECUTOR_HPP_
