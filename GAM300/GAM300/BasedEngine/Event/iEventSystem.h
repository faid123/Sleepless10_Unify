#pragma once
#include "Event/Event.h"
#include "Event/EventEmitter.h"
#include <unordered_set>
//Interface class for Systems to handle Events
//All systems that wish to handle events should inherit from this class
class iEventSystem
{
public:
	//Pubish event
	void publishEvent(const iEvent& event);

	//Clear all IDs from the set of subscribers
	void clearSubscriberID();

	//Subscribe to an event
	template <typename EventType>
	SubscriberID subscribeEvent(const std::function<void(const iEvent&)>& e_callback)
	{
		SubscriberID id = E_Emitter.subscribe<EventType>(e_callback);
		_subscriberIDs.insert(id);
		return id;
	}

	//Unsubscribe a subscribe from an event, takes in the Subscriber's ID
	template <typename EventType>
	void unsubscribeEvent(SubscriberID id)
	{
		if (_subscriberIDs.find(id) == _subscriberIDs.end())
			return;
		_subscriberIDs.erase(id);
		E_Emitter.unsubscribe<EventType>(id);
	}

	//Unsubscribe all subscribers pertaining to the specified event
	template <typename EventType>
	void unsubscribeAll()
	{
		for (auto id : _subscriberIDs)
			E_Emitter.unsubscribe<EventType>(id);
		_subscriberIDs.clear();
	}

private:
	std::unordered_set<SubscriberID> _subscriberIDs;
};