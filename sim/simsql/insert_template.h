#ifndef INSERT_TEMPLATE_H
#define INSERT_TEMPLATE_H
#include <simsql/mysql.h>
#include <simsql/mysql_escape.h>
#include <simsql/sim_utils.h>
namespace sim
{
namespace mysql
{

template<class OBJ>
class SqlInsert
{
public:
    SqlInsert(OBJ& o, Mysql* s=NULL):obj_(o),escape_(s)
    {
    }
    OBJ& Table(const std::string& table){table_ = table; return obj_;}
    OBJ& Insert(const std::string& name, const std::string& value);
    OBJ& Insert(const std::string& name, uint64_t value);
    const std::string& ToString();
    void Clear();
private:
    OBJ& obj_;
    Escape escape_;
    std::string query_;
    std::string begin_;
    std::string end_;
    std::string table_;
};


template<class OBJ>
OBJ& SqlInsert<OBJ>::Insert(const std::string& name, const std::string& value)
{
    begin_ += name + ",";
    end_ += "'" + escape_(val) + "'";
    return obj_;
}

template<class OBJ>
OBJ& SqlInsert<OBJ>::Insert(const std::string& name, uint64_t value)
{
    begin_ += name + ",";
    end_ += AtoStr(value) + ",";
    return obj_;
}

template<class OBJ>
const std::string& SqlInsert<OBJ>::ToString()
{
    if (query_.empty() && !begin_.empty())
    {
        begin_.erase(begin_.length()-1);
        end_.erase(end_.length()-1);
        query_ = "insert into " + table_ + "(" 
                 + begin_ + ")values(" + end_ + ")";
    }

    return query_;
}

template<class OBJ>
void SqlInsert<OBJ>::ClearInsert()
{
    query_.clear();
    begin_.clear();
    end_.clear();
}

}//endof namespace mysql
}//endof namespace sim

#endif

