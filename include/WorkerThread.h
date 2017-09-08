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

    ~WorkerThread();

private:
    bool m_terminate;
    bool m_processed;

    std::optional<Function> m_job;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::thread m_thread;


    void workerLoop();
};