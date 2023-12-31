#include "EventEmitter.h"

/// <summary>
/// The class instance of the event emitter
/// </summary>
EventEmitter E_Emitter;

/// <summary>
/// The destructor of the class
/// </summary>
EventEmitter::~EventEmitter()
{
	unsubscribeAll();
}

/// <summary>
/// Function to call to unsubscribe from all events
/// </summary>
void EventEmitter::unsubscribeAll()
{
	_subscribers.clear();
}

/// <summary>
/// This function publishes a specific event
/// </summary>
/// <param name="event"></param>
void EventEmitter::publish(const iEvent& event)
{
	std::pair<S_Map::iterator, S_Map::iterator> iter = _subscribers.equal_range(&typeid(event));
	for (S_Map::iterator it = iter.first; it != iter.second; ++it)
		it->second.second(event);
}