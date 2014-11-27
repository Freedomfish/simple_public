#ifndef OBSERVER_H
#define OBSERVER_H

#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>
#include <IceStorm/IceStorm.h>

namespace sim
{

template <typename T>
class Observer
{
public:
    Observer()
     :topic_(NULL),
      publisher_(NULL),
      subscriber_(NULL),
      publish_prx_(NULL)
    {}
    ~Observer(){}
    int Init(Ice::CommunicatorPtr communicator,
              const std::string& topicName);
    void Destroy()
    {
        //    shutdownOnInterrupt();
        if (topic_ && subscriber_) topic_->unsubscribe(subscriber_);
    }
    void StartSubscriber(const std::string& adapter_name,
                         const Ice::ObjectPtr& objptr);
    T publish_prx();
private:
    Ice::CommunicatorPtr communicator_;
    IceStorm::TopicPrx topic_;
    Ice::ObjectPrx publisher_;
    Ice::ObjectPrx subscriber_;
    T publish_prx_;
};

template<typename T>
int Observer<T>::Init(Ice::CommunicatorPtr communicator,
                    const std::string& topicName)
{
    communicator_ = communicator;


    IceStorm::TopicManagerPrx manager = IceStorm::TopicManagerPrx::checkedCast(
        communicator_->propertyToProxy("TopicManager.Proxy"));
    if(!manager)
    {
        std::cout<<__FILE__<<"|"<<__LINE__<<std::endl;
        return EXIT_FAILURE;
    }

    //
    // Retrieve the topic.
    //
    try
    {
        topic_ = manager->retrieve(topicName);
    }
    catch(const IceStorm::NoSuchTopic&)
    {
        try
        {
            topic_ = manager->create(topicName);
        }
        catch(const IceStorm::TopicExists&)
        {
            std::cout<<__FILE__<<"|"<<__LINE__<<std::endl;
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

template<typename T>
void Observer<T>::StartSubscriber(const std::string& adapter_name,
                                  const ::Ice::ObjectPtr& objptr)
{
    Ice::ObjectAdapterPtr adapter = communicator_->createObjectAdapter(adapter_name);
    Ice::Identity subId;
    subId.name = IceUtil::generateUUID();
    subscriber_ = adapter->add(objptr, subId);
    adapter->activate();
    
    enum Option { None, Datagram, Twoway, Oneway, Ordered};
    Option option = None;
    bool batch = false;
    std::string retryCount;
    IceStorm::QoS qos;
    if(!retryCount.empty())
    {
        qos["retryCount"] = retryCount;
    }

    //
    // Set up the proxy.
    //
    if(option == Datagram)
    {
        if(batch)
        {
            subscriber_ = subscriber_->ice_batchDatagram();
        }
        else
        {
            subscriber_ = subscriber_->ice_datagram();
        }
    }
    else if(option == Twoway)
    {
        // Do nothing to the subscriber proxy. Its already twoway.
    }
    else if(option == Ordered)
    {
        // Do nothing to the subscriber proxy. Its already twoway.
        qos["reliability"] = "ordered";
    }
    else if(option == Oneway || option == None)
    {
        if(batch)
        {
            subscriber_ = subscriber_->ice_batchOneway();
        }
        else
        {
            subscriber_ = subscriber_->ice_oneway();
        }
    }

    try
    {
        topic_->subscribeAndGetPublisher(qos, subscriber_);
    }
    catch(const IceStorm::AlreadySubscribed&)
    {
        // If we're manually setting the subscriber id ignore.
        std::cout << "reactivating persistent subscriber" << std::endl;
    }
    std::cout<<"Subscriber Start!"<<std::endl;
}

template<typename T>
T Observer<T>::publish_prx()
{
    if (!publish_prx_)
    {
        std::cout<<__FILE__<<"|"<<__LINE__<<std::endl;
//        enum Option { None, Datagram, Twoway, Oneway, Ordered};
  //      Option option = None;
        publisher_ = topic_->getPublisher();
        //if(option == Datagram)
        //{
        //    publisher_ = publisher_->ice_datagram();
        //}
        //else if(option == Twoway)
        //{
        //    // Do nothing.
        //}
        //else if(option == Oneway || option == None)
        //{
        //    publisher_ = publisher_->ice_oneway();
        //}

        publish_prx_ = T::uncheckedCast(publisher_);
    }
   
    if (!publish_prx_)
    {
        std::cout<<__FILE__<<"|"<<__LINE__<<std::endl;
    }
    return publish_prx_;
}

}

#endif
