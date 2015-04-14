#include <RedisCache/RedisCache.h>
using namespace std;
using namespace sim;
std::string RedisCache::Get(const std::string& key)
{
    string value;
    SimRedisReply r = redis_->TryCommand("GET %s", key);
    if (r && r->str) value.append(r->str, r->len);
    return value;
}

void RedisCache::Set(const std::string& key,
                     const std::string& value,
                     int64_t expire)
{
    vector<vector<string>> argvs;
    argvs.push_back(command("SET")(key)(value));    
    if (expire) argvs.push_back(command("EXPIRE")(key)(expire));
    redis_->TryAppendArgvs(argvs);
}

void RedisCache::Remove(const std::string& key)
{
    string value;
    SimRedisReply r = redis_->TryCommand("DEL %s", key);
    r.Free();
}

