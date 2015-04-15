#ifndef SIM_TYPEDEF_H
#define SIM_TYPEDEF_H


#include <boost/function.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/detail/thread.hpp>
#include <boost/thread/thread_time.hpp>

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <memory>
#include <sstream>

#define SharedPtr std::shared_ptr
#define WeakPtr boost::weak_ptr
#define SimBind boost::bind
#define SimFunction boost::function
#define SimThread boost::thread

namespace sim {

inline std::string NewUuid()
{
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    std::stringstream ss;
    ss << uuid;
    return ss.str();
}

//#define SharedPtr boost::shared_ptr
typedef boost::mutex Mutex;
typedef boost::unique_lock<Mutex> ScopeLock;
typedef boost::condition_variable ConditionVar;

typedef boost::shared_mutex            SharedMutex;  
typedef boost::unique_lock<SharedMutex>   WriteLock;  
typedef boost::shared_lock<SharedMutex>   ReadLock;

template<class T>
class SimNoLock
{
public:
    SimNoLock():ix_(0){}
 
    virtual void Update()
    {
        int i = ix_ ^ 1;
        ix_ = i;
    }
protected:
    T t_[2];
    int ix_;
};

}

#endif
