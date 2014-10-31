#include <sim/string_common.h>
#include <sim/simexception.h>
#include <sim/simini.h>

#include <mysql_route_factory.h>
#include <mysql_route_common.h>
#include <mysql_common_str.h>
using namespace std;
using namespace sim;

#define TEST "test"

enum DBType
{
    DB_TEST = 0
};

enum TableType
{
    TABLE_TEST = 0
};

class TestMysql : public MysqlRouteCommon<TestMysql>
{
public:
    int TestWrite()
    {
        int id = 10;
        string name = "Test";
        string dbname = split_handler()->DbName(id, DB_TEST);
        string tbname = split_handler()->TableName(id, TABLE_TEST);
        int index = split_handler()->Index(id, DB_TEST);
        MysqlObj obj = route()->get(index, TEST, WRITE);
        if (!obj) return 1;
        SqlInsertStr tmp_sql;
        tmp_sql.set_table(dbname + "." + tbname);
        tmp_sql.add_num("id", id);
        tmp_sql.add_str("name", name);
        string sql = tmp_sql.ToStr();
        cout<<"sql="<<sql;
        obj->TryQuery(sql);
        if (obj->errcode())
            cout<<"errmsg="<<obj->errmsg()<<endl;
        return obj->errcode();
    }
    int TestRead()
    {
        int id = 10;
        string name;
        string dbname = split_handler()->DbName(id, DB_TEST);
        string tbname = split_handler()->TableName(id, TABLE_TEST);
        int index = split_handler()->Index(id, DB_TEST);
        MysqlObj obj = route()->get(index, TEST, WRITE);
        if (!obj) return 1;
        
        const char* sqlfmt = "select name from %s.%s where id=%d limit 1";
        char sql[255] = {0};
        snprintf(sql, sizeof(sql), sqlfmt, dbname.c_str(), tbname.c_str(), id);
        cout<<"sql="<<sql<<endl;
        SimSqlRes res = obj->TryQueryRes(sql);
        int cnt = res.RowNum();
        for (int i=0; i<cnt; ++i)
        {
            MYSQL_ROW row = res.FetchRow();
            unsigned long* rowlens = res.FetchLengths();
            if (!row) continue;
            if (row[0]) name = string(row[0], rowlens[0]);
        }
        if (obj->errcode())
            cout<<"errmsg="<<obj->errmsg()<<endl;
        return obj->errcode();
    }
};

class TestSplitRule : public SplitDbTableRule
{
public:
    int Index(const int i, const int type_i) const
    {
        return i%10;
    }
    std::string DbName(const int i, const int type_i) const
    {
        static std::string dbname_arr = {
            "test_db"
        };
        return dbname_arr[type_i] + "_" + atostr(i%10);
    }
    std::string TableName(const int i, const int type_i) const
    {
        static std::string tbname_arr = {
            "test_table"
        };
        return tbname_arr[type_i] + "_" + atostr(i%10);
    }
};

int main(void)
{
    SimIni ini;
    if (ini.LoadFile("config.ini"))
    {
        cout<<"errno="<<errno<<endl;
    }
    vector<string> types;
    types.push_back(TEST);
    try
    {
        MysqlRouteBaseFactory *factory = new MysqlRouteFactory;
        SplitDbTableRule* ruleh = new TestSplitRule;
        TestMysql::Init(ruleh, factory);
        TestMysql::InitData(ini, types);
    }
    catch (const SimException& e)
    {
        cout<<"err="<<e.what()<<endl;
        return 1;
    }
    TestMysql test_sql;
    test_sql.TestRead();
    test_sql.TestWrite();
    return 0;
}


