#ifndef _SIMINI_H
#define _SIMINI_H

#include <map>
#include <string>
namespace sim {
class SimIni
{
/*key-val*/
typedef std::map<std::string, std::string> key_val_map;
/*session - (key-val)*/
typedef std::map<std::string, key_val_map> sess_map;
public:
    SimIni();
    ~SimIni();
    inline bool is_loaded(){return m_is_loaded;}
    inline void clear(){m_sess_map.clear();}
    int LoadFile(const char* file);
    const std::string& GetStringVal(
            const std::string& sess,
            const std::string& key) const;
    const std::string& GetStringValWithDefault(
            const std::string& sess,
            const std::string& key,
            const std::string& defstr) const;
    int GetIntVal(
            const std::string& sess,
            const std::string& key) const;
    int GetIntValWithDefault(
            const std::string& sess,
            const std::string& key,
            const int defi) const;

    std::map<std::string, std::string> GetSession(
            const std::string& sess) const;    

    int GetStringVal(const std::string& sess,
            const std::string& key,
            std::string& val) const;
    void AddValue(const std::string& sess,
                      const std::string& key,
                      const std::string& val);
    std::string ToString();
public:
    void test();
private:
    bool m_is_loaded;
    sess_map m_sess_map;
};

class SingleSimIni
{
public:
    static SimIni& get();
private:
    static SimIni s_ini;
};

}

#endif
