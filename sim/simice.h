/* 
 * File:   friendcircle_common.h
 * Author: yujintao
 *
 * Created on 2014年4月17日, 上午11:45
 */

#ifndef ICE_COMMON_H
#define ICE_COMMON_H
#include <Ice/Ice.h>
#include <IceBox/IceBox.h>
#include <IceUtil/IceUtil.h>
#include <IceGrid/IceGrid.h>
#include "log4cplus_common.h"
#include "simini.h"

namespace sim {

struct IceClientObject
{
    std::string objid;
    std::string objtype;
    std::string regquery;
    std::string locator;
};

struct IceClientObjectProperty
{
    std::string objid;
    std::string objtype;
    std::string regquery;
    std::string locator;
};

inline void IceObjInitFromIni(const SimIni& ini, 
        const std::string& sess, 
    IceClientObject& obj)
{
    obj.regquery = ini.GetStringValWithDefault(sess, "regquery","");
    obj.objid = ini.GetStringValWithDefault(sess, "objid", "");
    obj.objtype = ini.GetStringValWithDefault(sess, "objtype", "");
    obj.locator = ini.GetStringValWithDefault(sess, "Locator", "");
}

inline void IceObjptyInitFromIni(const SimIni& ini, 
        const std::string& sess, 
        IceClientObject& obj, 
        IceClientObjectProperty& objpty)
{
    objpty.objid = sess + "_objid";
    objpty.objtype = sess + "_objtype";
    objpty.regquery = sess + "_regquery";
    objpty.locator = objpty.objid + "." + "Locator";
}


inline void InitSetProperty(Ice::InitializationData& data, IceClientObject& obj, IceClientObjectProperty& objpty)
{
    data.properties->setProperty(objpty.locator, obj.locator);
    data.properties->setProperty(objpty.objid, obj.objid);
    data.properties->setProperty(objpty.objtype, obj.objtype);
    data.properties->setProperty(objpty.regquery, obj.regquery);
}

class IceClientCommon
{
public:
    IceClientCommon(){}
    IceClientCommon(Ice::CommunicatorPtr& commu):communicator(commu){}
    void set_communicator(Ice::CommunicatorPtr commu){communicator = commu;}
    template<typename Prx> 
    int GetPrx(Prx &prx, const std::string& objid, const std::string& regquery, 
        const std::string& objtype)
    {
        try 
        {
            prx = Prx::checkedCast(communicator->stringToProxy(objid));
        }
        catch(const Ice::NotRegisteredException&) 
        {
            try 
            {
                IceGrid::QueryPrx query = IceGrid::QueryPrx::checkedCast(communicator->stringToProxy(regquery));
                prx = Prx::checkedCast(query->findObjectByType(objtype));
            }
            catch(std::exception& e) 
            {
                LOG_APP_ERROR("query|objid=%s|regquery=%s|objtype=%s|errmsg=%s", 
                    objid.c_str(), regquery.c_str(), objtype.c_str(), e.what());
                return -1;
            }
        }
        catch(std::exception& e) 
        {
            LOG_APP_ERROR("objid=%s|regquery=%s|objtype=%s|errmsg=%s", 
                objid.c_str(), regquery.c_str(), objtype.c_str(), e.what());
            return -1;
        }

        return 0;

    }

    template<typename Prx>
    int GetPrxByProperty(Prx& prx, const std::string& objid, const std::string& regquery, const std::string& objtype) 
    {
        try 
        {
            prx = Prx::checkedCast(communicator->propertyToProxy(objid));
        } 
        catch (const Ice::NotRegisteredException& e) 
        {
            LOG_APP_ERROR("query|objid=%s|regquery=%s|objtype=%s|errmsg=%s", 
                    objid.c_str(), regquery.c_str(), objtype.c_str(), e.what());
            try
            {
                IceGrid::QueryPrx query = IceGrid::QueryPrx::checkedCast(communicator->propertyToProxy(regquery));
                prx = Prx::checkedCast(query->findObjectByType(communicator->getProperties()->getProperty(objtype)));
            }
            catch(std::exception& e) 
            {
                LOG_APP_ERROR("objid=%s|regquery=%s|objtype=%s|errmsg=%s", 
                    objid.c_str(), regquery.c_str(), objtype.c_str(), e.what());
                return -1;
            }
        }
        catch (std::exception& e)
        {
            return -1;
        }

        return prx;
    }

    template<typename Prx>
    Prx GetPrx(const IceClientObject& obj)
    {
        Prx prx;
        GetPrx(prx, obj.objid, obj.regquery, obj.objtype);
        return prx;
    }
    template<typename Prx>
    Prx GetPrxByProperty(const IceClientObjectProperty& obj)
    {
        Prx prx;
        GetPrxByProperty(prx,obj.objid, obj.regquery, obj.objtype);
        return prx;
    }

private:
    Ice::CommunicatorPtr communicator;
};

}

#endif    /* FRIENDCIRCLE_COMMON_H */

