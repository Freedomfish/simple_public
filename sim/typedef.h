#ifndef SIM_TYPEDEF_H
#define SIM_TYPEDEF_H

#include <boost/shared_ptr.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>


#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

#include <memory>
#include <sstream>

namespace sim {

inline std::string NewUuid()
{
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    std::stringstream ss;
    ss << uuid;
    return ss.str();
}

//#define SharedPtr boost::shared_ptr
#define SharedPtr std::shared_ptr
typedef boost::mutex Mutex;
typedef boost::unique_lock<Mutex> ScopeLock;

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
