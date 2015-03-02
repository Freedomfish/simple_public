#ifndef MYSQL_UPDATE_H
#define MYSQL_UPDATE_H

class MysqlUpdate : public SqlFilter<MysqlUpdate>,
{
public:
    SqlUpdate<MysqlUpdate> Update;
public:
    MysqlUpdate(Mysql* s=NULL):SqlFilter<MysqlUpdate>(*this, s), sql_(s), updater_(*this, s)
    {
    }

    int Query();
    MysqlUpdate& operator (const std::string& table){Update(table); return *this;}
    MysqlUpdate& Table(const std::string& table){Update(table); return *this;}
    MysqlInsert& Use(const std::string& db){sql_->Query("use " + db); return *this;}
private:
    Mysql* sql_;
};

inline int MysqlUpdate::Query()
{
    return sql_->Query(Update.ToString() + " where " + ToString());
}

#endif
