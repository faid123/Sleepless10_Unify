#pragma once
#include <functional>
#include <map>
#include <typeinfo>
#include "Event.h"

using SubscriberID = uint32_t;
typedef std::multimap<const std::type_info*, std::pair<SubscriberID, const std::function<void(const iEvent&)>>> S_Map;

class EventEmitter
{
public:
	~EventEmitter();
	//Subscribe to an event
	template <typename EventType>
	SubscriberID subscribe(const std::function<void(const iEvent&)>& e_callback)
	{
		_subscribers.insert(std::make_pair(&typeid(EventType), std::make_pair(_sID, e_callback)));
		return _sID++;
	}
	//Unsubscribe to an event, pass in an ID as a parameter. 
	template <typename EventType>
	void unsubscribe(SubscriberID id)
	{
		std::pair<S_Map::iterator, S_Map::iterator> iter = _subscribers.equal_range(&typeid(EventType));
		for (S_Map::iterator it = iter.first; it != iter.second;)
		{
			if (it->second.first == id)
				it = _subscribers.erase(it);
			else
				++it;
		}
	}
	//Unsubscribe from all events
	void unsubscribeAll();

	//Publish event
	void publish(const iEvent& event);
private:
	S_Map _subscribers;
	SubscriberID _sID = 0;
};

extern EventEmitter E_Emitter;