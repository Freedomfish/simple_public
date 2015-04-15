#include <sim/thread/thread_safe_queue.h>
using namespace sim;

ThreadSafeQueue::ThreadSafeQueue(size_t threadNum, size_t maxQueueSize)
{
    queues_.reserve(threadNum);
    for (size_t i=0; i<threadNum; ++i)
    {
        ThreadQueuePtr tq(new ThreadQueue());
        tq->set_maxsize(maxQueueSize);
        tq->set_threadcount(1);
        queues_.push_back(tq);
    }
    threadNum_ = threadNum;
}

void ThreadSafeQueue::start()
{
    std::vector<ThreadQueuePtr>::iterator it;
    for (it=queues_.begin(); it!=queues_.end(); ++it)
        (*it)->start();
}

void ThreadSafeQueue::stop()
{
    std::vector<ThreadQueuePtr>::iterator it;
    for (it=queues_.begin(); it!=queues_.end(); ++it)
        (*it)->stop();
}

int ThreadSafeQueue::push_back(size_t id, const ThreadHandlerPtr& h)
{
    queues_[id%threadNum_]->push_back(h);
}

 
