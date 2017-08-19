#pragma once

#include <functional>
#include <optional>
#include <mutex>
#include <thread>

class WorkerThread
{
public:
    using Function = std::function<void()>;

    WorkerThread();

    void setJob(Function func);

    void wait();

private:
    std::optional<Function> m_job;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    bool m_processed = false;
    std::thread m_thread;

    void workerLoop();
};