#include "WorkerThread.h"

WorkerThread::WorkerThread() :
    m_terminate(false),
    m_processed(false),
    m_thread([this]() { workerLoop(); })
{

}

void WorkerThread::setJob(Function func)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_processed = false;
    m_job = func;
    lock.unlock();
    m_cv.notify_one();
}

void WorkerThread::wait()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cv.wait(lock, [this] {return m_processed; });
    lock.unlock();
}
WorkerThread::~WorkerThread()
{
    m_terminate = true;
    m_cv.notify_one();
    m_thread.join();
}

void WorkerThread::workerLoop()
{
    for (;;)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait(lock, [this] {return m_job.has_value() || m_terminate; });
        if (m_terminate) break;

        m_job.value()();
        m_job.reset();
        m_processed = true;

        lock.unlock();
        m_cv.notify_one();
    }
}