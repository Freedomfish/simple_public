#ifndef UPDATE_TEMPLATE_H
#define UPDATE_TEMPLATE_H
#include <simsql/mysql.h>
#include <simsql/mysql_escape.h>
#include <simsql/sim_utils.h>
namespace sim
{
namespace mysql
{

template<class OBJ>
class SqlUpdate
{
public:
    SqlUpdate(OBJ& o, Mysql* s=NULL):obj_(o),escape_(s){}
    OBJ& Table(const std::string& table){table_=table; return obj_;}
    OBJ& Update(const std::string& name, const std::string& value);
    OBJ& Update(const std::string& name, uint64_t value);
    const std::string& ToString();

    OBJ& operator (const std::string& table){return Table(table);}
    OBJ& operator (const std::string& name, const std::string& value){return Update(name, value);}
    OBJ& operator (const std::string& name, uint64_t value){return Update(name, value);}

    void Clear(){table_.clear(); field_.clear();};
    
private:
    OBJ& obj_;
    Escape escape_;
    std::string table_;
    std::string field_;
};

template<class OBJ>
OBJ& SqlUpdate<OBJ>::Update(const std::string& name, const std::string& value)
{
    field_ += name + "='" + escape_(value) + "',";
    return obj_;
}

template<class OBJ>
OBJ& SqlUpdate<OBJ>::Update(const std::string& name, uint64_t value)
{
    field_ += name + "=" + AtoStr(value) + "," 
    return obj_;
}

template<class OBJ>
const std::string& SqlUpdate<OBJ>::ToString()
{
    if (!field_.empty())
    {
        return "update " + table + " set " + field_.substr(field_.length()-1);
    }
    return "";
}

}//end of namespace mysql
}//end of namespace sim

#endif
