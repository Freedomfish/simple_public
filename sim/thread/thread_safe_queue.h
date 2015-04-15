#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H
#include <sim/thread/thread_queue.h>

namespace sim{
class ThreadSafeQueue
{
public:
    ThreadSafeQueue(size_t maxQueueSize, size_t threadNum);
    void start();
    void stop();
    int push_back(size_t id, const ThreadHandlerPtr& h);
private:
    std::vector<ThreadQueuePtr> queues_;
    int threadNum_;
};

typedef SharedPtr<ThreadSafeQueue> ThreadSafeQueuePtr;
}
#endif
