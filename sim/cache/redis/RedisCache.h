#ifndef REDIS_CACHE_H
#define REDIS_CACHE_H
#include <sim/cache/Cache.h>
#include <sim/simredis.h>
#include <sim/typedef.h>
namespace sim {
typedef SharedPtr<SimRedis> SimRedisPtr;
class RedisCache : public CacheInf
{
public:
    RedisCache(const SimRedisPtr& p):redis_(p){}
    std::string Get(const std::string& key);
    void Set(const std::string& key, const std::string& value, int64_t expire = 0);
    void Remove(const std::string& key);
    int errcode() {return redis_->errcode();}
    std::string errmsg(){return redis_->errmsg();}
private:
    SimRedisPtr redis_;
};

typedef SharedPtr<RedisCache> RedisCachePtr;
}
#endif
